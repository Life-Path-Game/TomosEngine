#include "Renderer.hh"
#include "Tomos/util/logger/Logger.hh"

namespace Tomos
{
    static std::shared_ptr<VertexArray> g_screenQuadVAO;
    static std::shared_ptr<Shader>      g_defaultPostProcessShader;

    std::unordered_map<MaterialBatchKey,
                       std::unordered_map<std::shared_ptr<VertexArray>, BatchData>,
                       MaterialBatchKeyHash> Renderer::g_batches;
    bool                                     Renderer::g_batching = false;

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

    void Renderer::endBatch( const glm::mat4& viewProjection )
    {
        if ( !g_batching ) return;

        for ( auto& [materialKey, meshBatches] : g_batches )
        {
            // Bind shader and material once per group
            materialKey.shader->bind();
            materialKey.material->bind();
            materialKey.shader->setMat4( "uViewProjection", viewProjection );

            for ( auto& [vertexArray, batchData] : meshBatches )
            {
                vertexArray->bind();

                // Draw all instances with this shader/material/VAO combination
                for ( const auto& transform : batchData.transforms )
                {
                    materialKey.shader->setMat4( "uTransform", transform );

                    glDrawElements( GL_TRIANGLES,
                                    vertexArray->getIndexBuffer()->getCount(),
                                    GL_UNSIGNED_INT,
                                    nullptr );
                }
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
        if ( !batchData.vertexArray )
        {
            batchData.vertexArray = p_vertexArray;
        }
        batchData.transforms.push_back( p_transform );
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

    void Renderer::beginFrameBufferRender( const std::shared_ptr<FrameBuffer>& framebuffer )
    {
        if ( framebuffer )
        {
            framebuffer->bind();
            glClearColor( 0.0f, 0.0f, 0.0f, 0.0f ); // Clear with transparent black
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        }
    }

    void Renderer::endFrameBufferRender()
    {
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    }

    void Renderer::renderFrameBuffer( const std::shared_ptr<FrameBuffer>& framebuffer,
                                      const std::shared_ptr<Shader>&      postProcessShader )
    {
        if ( !framebuffer ) return;

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
        auto shaderToUse = postProcessShader ? postProcessShader : g_defaultPostProcessShader;
        if ( !shaderToUse )
        {
            // Create default post-process shader if not exists
            g_defaultPostProcessShader = std::make_shared<Shader>(
                    ResourceManager::getShaderPath( "screen_quad_vertex.glsl" ),
                    ResourceManager::getShaderPath( "screen_quad_fragment.glsl" )
                    );
            shaderToUse = g_defaultPostProcessShader;
        }

        shaderToUse->bind();
        framebuffer->getColorTexture()->bind( 0 );
        shaderToUse->setInt( "uScreenTexture", 0 );

        g_screenQuadVAO->bind();
        glDrawArrays( GL_TRIANGLES, 0, 6 );
        glDisable( GL_BLEND );
        glEnable( GL_DEPTH_TEST );
    }
}
