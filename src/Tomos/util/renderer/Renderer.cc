#include "Renderer.hh"

#include "Tomos/core/Application.hh"
#include "Tomos/util/logger/Logger.hh"

namespace Tomos
{
    static std::shared_ptr<VertexArray> g_screenQuadVAO;
    static std::shared_ptr<Shader>      g_defaultPostProcessShader;

    std::unordered_map<MaterialBatchKey,
                       std::unordered_map<std::shared_ptr<VertexArray>, BatchData>,
                       MaterialBatchKeyHash> Renderer::g_batches;
    bool                                     Renderer::g_batching = false;

    std::unordered_map<std::shared_ptr<VertexArray>,
                       std::shared_ptr<StorageBuffer>> Renderer::g_instanceBuffers;

    void Renderer::setClearedColor( const glm::vec4& p_color )
    {
        glClearColor( p_color.r, p_color.g, p_color.b, p_color.a );
    }

    void Renderer::clear( unsigned int p_mask )
    {
        glClear( p_mask );
    }

    void Renderer::onWindowResize( unsigned int p_width, unsigned int p_height )
    {
        // glViewport(0, 0, p_width, p_height);
    }

    void Renderer::beginBatch()
    {
        g_batching = true;
        g_batches.clear();
    }

    void Renderer::endBatch( const glm::mat4& p_viewProjection )
    {
        if ( !g_batching ) return;

        for ( auto& [materialKey, meshBatches] : g_batches )
        {
            materialKey.m_shader->bind();
            materialKey.m_material->bind();
            materialKey.m_shader->setMat4( "uViewProjection", p_viewProjection );

            for ( auto& [vertexArray, batchData] : meshBatches )
            {
                // Convert batch to instanced rendering
                std::vector<InstanceData> instances;
                instances.reserve( batchData.m_transforms.size() );
                for ( const auto& transform : batchData.m_transforms )
                {
                    instances.emplace_back( InstanceData{transform} );
                }

                drawInstanced( materialKey.m_shader,
                               materialKey.m_material,
                               vertexArray,
                               instances,
                               p_viewProjection,
                               true );
            }
        }

        g_batching = false;
        g_batches.clear();
    }

    void Renderer::addToBatch( const std::shared_ptr<Shader>&      p_shader,
                               const std::shared_ptr<Material>&    p_material,
                               const std::shared_ptr<VertexArray>& p_vertexArray,
                               const glm::mat4&                    p_transform )
    {
        if ( !g_batching ) return;

        MaterialBatchKey key{p_shader, p_material};
        auto&            batchData = g_batches[key][p_vertexArray];
        if ( !batchData.m_vertexArray )
        {
            batchData.m_vertexArray = p_vertexArray;
        }
        batchData.m_transforms.push_back( p_transform );
    }

    void Renderer::draw( const std::shared_ptr<Shader>&      p_shader,
                         const std::shared_ptr<Material>&    p_material,
                         const std::shared_ptr<VertexArray>& p_vertexArray,
                         const glm::mat4&                    p_transform,
                         const glm::mat4&                    p_viewProjection )
    {
        if ( g_batching )
        {
            addToBatch( p_shader, p_material, p_vertexArray, p_transform );
            return;
        }

        // Fallback to immediate mode if not batching
        if ( !p_vertexArray || !p_vertexArray->getIndexBuffer() )
        {
            LOG_ERROR() << "Renderer::draw() - Invalid VertexArray or IndexBuffer";
            return;
        }

        p_shader->bind();
        p_material->bind();
        p_shader->setMat4( "uTransform", p_transform );
        p_shader->setMat4( "uViewProjection", p_viewProjection );

        p_vertexArray->bind();
        glDrawElements( GL_TRIANGLES,
                        p_vertexArray->getIndexBuffer()->getCount(),
                        GL_UNSIGNED_INT,
                        nullptr );

        // Error checking
        GLenum err;
        while ( ( err = glGetError() ) != GL_NO_ERROR )
        {
            LOG_ERROR() << "OpenGL error in Renderer::draw(): " << err;
        }
    }

    void Renderer::drawInstanced( const std::shared_ptr<Shader>&      p_shader,
                                  const std::shared_ptr<Material>&    p_material,
                                  const std::shared_ptr<VertexArray>& p_vertexArray,
                                  const std::vector<InstanceData>&    p_instances,
                                  const glm::mat4&                    p_viewProjection,
                                  bool                                p_batched
            )
    {
        if ( !p_vertexArray || !p_vertexArray->getIndexBuffer() )
        {
            LOG_ERROR() << "Invalid VertexArray or IndexBuffer";
            return;
        }

        if ( !p_batched )
        {
            p_shader->bind();
            p_material->bind();
        }

        p_shader->setMat4( "uViewProjection", p_viewProjection );

        p_vertexArray->bind();

        size_t instanceCount  = p_instances.size();
        size_t instancesDrawn = 0;

        auto maxInstances = Application::getState().config().get<size_t>( "maxInstancesPerDraw" );

        while ( instancesDrawn < instanceCount )
        {
            size_t batchSize = std::min( maxInstances, instanceCount - instancesDrawn );

            auto& instanceBuffer = g_instanceBuffers[p_vertexArray];
            if ( !instanceBuffer || instanceBuffer->getSize() < sizeof( InstanceData ) * maxInstances )
            {
                instanceBuffer = std::make_shared<StorageBuffer>( sizeof( InstanceData ) * maxInstances );
            }

            instanceBuffer->setData(
                    p_instances.data() + instancesDrawn,
                    sizeof( InstanceData ) * batchSize
                    );

            instanceBuffer->bindBase( 0 ); // Binding point 0

            glDrawElementsInstanced(
                    GL_TRIANGLES,
                    p_vertexArray->getIndexBuffer()->getCount(),
                    GL_UNSIGNED_INT,
                    nullptr,
                    static_cast<GLsizei>( batchSize )
                    );

            instancesDrawn += batchSize;
        }

        // Error checking
        GLenum err;
        while ( ( err = glGetError() ) != GL_NO_ERROR )
        {
            LOG_ERROR() << "OpenGL error: " << err;
        }
    }

    void Renderer::beginFrameBufferRender( const std::shared_ptr<FrameBuffer>& p_framebuffer )
    {
        if ( p_framebuffer )
        {
            p_framebuffer->bind();
        }
    }

    void Renderer::endFrameBufferRender()
    {
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    }

    void Renderer::renderFrameBuffer( const std::shared_ptr<FrameBuffer>& p_framebuffer,
                                      const std::shared_ptr<Shader>&      p_postProcessShader )
    {
        if ( !p_framebuffer ) return;

        glDisable( GL_DEPTH_TEST );
        glEnable( GL_BLEND );
        glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );

        // Initialize screen quad if not already done
        if ( !g_screenQuadVAO )
        {
            float quadVertices[] = {
                    // positions   // texCoords
                    -1.0f, 1.0f, 0.0f, 1.0f,
                    -1.0f, -1.0f, 0.0f, 0.0f,
                    1.0f, -1.0f, 1.0f, 0.0f,

                    -1.0f, 1.0f, 0.0f, 1.0f,
                    1.0f, -1.0f, 1.0f, 0.0f,
                    1.0f, 1.0f, 1.0f, 1.0f
            };

            auto vbo = std::make_shared<VertexBuffer>( quadVertices, sizeof( quadVertices ) );
            vbo->setLayout( {
                    {ShaderDataType::Float2, "aPosition"},
                    {ShaderDataType::Float2, "aTexCoord"}
            } );

            g_screenQuadVAO = std::make_shared<VertexArray>();
            g_screenQuadVAO->addVertexBuffer( vbo );
        }

        // Use default shader if none provided
        auto shaderToUse = p_postProcessShader ? p_postProcessShader : g_defaultPostProcessShader;
        if ( !shaderToUse )
        {
            // Create default post-process shader if not exists
            g_defaultPostProcessShader = ResourceManager::getShader( "default_post_process" );
            if ( !g_defaultPostProcessShader )
            {
                g_defaultPostProcessShader = std::make_shared<Shader>(
                        ResourceManager::getShaderPath( "screen_quad_vertex.glsl" ),
                        ResourceManager::getShaderPath( "screen_quad_fragment.glsl" )
                        );
                ResourceManager::cacheShader( "default_post_process", g_defaultPostProcessShader );
            }
            shaderToUse = g_defaultPostProcessShader;
        }

        shaderToUse->bind();
        p_framebuffer->getColorTexture()->bind( 0 );
        shaderToUse->setInt( "uScreenTexture", 0 );

        g_screenQuadVAO->bind();
        glDrawArrays( GL_TRIANGLES, 0, 6 );
        glDisable( GL_BLEND );
        glEnable( GL_DEPTH_TEST );
    }
}
