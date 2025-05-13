// RenderPass.hh
#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "Tomos/util/renderer/FrameBuffer.hh"

namespace Tomos
{
    class Shader;

    class RenderPass
    {
    public:
        virtual ~RenderPass() = default;

        virtual void apply( int p_layerId, const glm::mat4& p_viewProj ) = 0;
        virtual void onResize( int p_width, int p_height ) = 0;

        virtual std::shared_ptr<FrameBuffer> getOutput() const = 0;

        virtual void                    setShader( std::shared_ptr<Shader> p_shader ) { m_shader = p_shader; }
        virtual std::shared_ptr<Shader> getShader() const { return m_shader; }

    protected:
        std::shared_ptr<Shader> m_shader;
    };
}
