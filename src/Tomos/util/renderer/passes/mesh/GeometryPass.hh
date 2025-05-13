// GeometryPass.hh
#pragma once
#include "../RenderPass.hh"

namespace Tomos
{
    class GeometryPass : public RenderPass
    {
    public:
        GeometryPass();

        void                         apply( int p_layerId, const glm::mat4& p_viewProj ) override;
        void                         onResize( int p_width, int p_height ) override;
        std::shared_ptr<FrameBuffer> getOutput() const override { return m_buffer; }

    private:
        std::shared_ptr<FrameBuffer> m_buffer;
    };
}
