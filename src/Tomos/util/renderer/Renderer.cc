//
// Created by dstuden on 2/21/25.
//

#include "Renderer.hh"

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

    void Renderer::beginScene()
    {
    }

    void Renderer::endScene()
    {
    }

    void Renderer::draw( const std::shared_ptr<Shader>&      p_shader,
                         const std::shared_ptr<VertexArray>& p_vertexArray,
                         const glm::mat4&                    p_transform,
                         const glm::mat4&                    p_viewProjection )
    {
        p_shader->bind();
        p_shader->setMat4( "transform", p_transform );
        p_shader->setMat4( "viewProjection", p_viewProjection );

        p_vertexArray->bind();
        glDrawElements( GL_TRIANGLES, p_vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr );
    }
} // namespace Tomos
