#pragma once
#include <memory>
#include <glm/glm.hpp>

#include "Shader.hh"
#include "VertexArray.hh"

namespace Tomos
{
    class Renderer
    {
    public:
        static void setClearedColor( const glm::vec4& p_color );
        static void clear( unsigned int p_mask );

        static void beginScene();
        static void endScene();

        static void draw( const std::shared_ptr<Shader>&      p_shader,
                          const std::shared_ptr<VertexArray>& p_vertexArray,
                          const glm::mat4&                    p_transform,
                          const glm::mat4&                    p_viewProjection );
    };
} // namespace Tomos
