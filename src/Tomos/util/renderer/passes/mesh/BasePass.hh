// GeometryPass.hh
#pragma once
#include "../RenderPass.hh"

namespace Tomos
{
    /*
     This render pass is responsible for rendering geometry, sprites and other base objects.
     */
    class BasePass : public RenderPass
    {
    public:
        BasePass();

        void                         apply( int p_layerId, const glm::mat4& p_viewProj ) override;
        void                         onResize( int p_width, int p_height ) override;
        std::shared_ptr<FrameBuffer> getOutput() const override { return m_buffer; }

    private:
        std::shared_ptr<FrameBuffer> m_buffer;
    };
}
