//
// Created by dstuden on 2/21/25.
//

#include "Renderer.hh"

#include "Tomos/util/logger/Logger.hh"

namespace Tomos
{
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
        // glViewport( 0, 0, p_width, p_height );
    }

    void Renderer::beginScene()
    {
    }

    void Renderer::endScene()
    {
    }

    void Renderer::submit( const std::shared_ptr<Shader>&      p_shader,
                           const std::shared_ptr<VertexArray>& p_vertexArray,
                           const glm::mat4&                    p_transform,
                           const glm::mat4&                    p_viewProjection )
    {
    }

    void Renderer::draw( const std::shared_ptr<Shader>&      p_shader,
                         const std::shared_ptr<VertexArray>& p_vertexArray,
                         const glm::mat4&                    p_transform,
                         const glm::mat4&                    p_viewProjection )
    {
        if ( !p_vertexArray || !p_vertexArray->getIndexBuffer() )
        {
            LOG_ERROR() << "Renderer::draw() - Invalid VertexArray or IndexBuffer";
            return;
        }

        p_shader->bind();
        p_shader->setMat4( "uTransform", p_transform );
        p_shader->setMat4( "uViewProjection", p_viewProjection );

        p_vertexArray->bind();

        GLenum indexType = GL_UNSIGNED_INT; // default

        glDrawElements( GL_TRIANGLES,
                        p_vertexArray->getIndexBuffer()->getCount(),
                        indexType,
                        nullptr );

        // Check for OpenGL errors
        GLenum err;
        while ( ( err = glGetError() ) != GL_NO_ERROR )
        {
            LOG_ERROR() << "OpenGL error in Renderer::draw(): " << err;
        }
    }
} // namespace Tomos
