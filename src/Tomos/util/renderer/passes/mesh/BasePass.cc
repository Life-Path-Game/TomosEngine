// GeometryPass.cpp
#include "BasePass.hh"
#include "Tomos/core/Application.hh"
#include "Tomos/systems/camera/CameraSystem.hh"
#include "Tomos/systems/mesh/MeshSystem.hh"
#include "Tomos/util/renderer/Renderer.hh"

namespace Tomos
{
    BasePass::BasePass()
    {
        int width  = Application::get()->getWindow().getData().m_width;
        int height = Application::get()->getWindow().getData().m_height;

        m_buffer = std::make_shared<FrameBuffer>( width, height );
    }

    void BasePass::apply( int p_layerId, const glm::mat4& p_viewProj )
    {
        Renderer::beginFrameBufferRender( m_buffer );
        Renderer::setClearedColor( glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f ) );
        Renderer::clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        auto& ecs = Application::get()->getState().ecs();

        const auto& drawCalls = ecs.getSystem<MeshSystem>().getDrawCalls( p_layerId );

        Renderer::beginBatch();

        for ( const auto& drawCall : drawCalls )
        {
            Renderer::addToBatch(
                    drawCall.m_shader,
                    drawCall.m_material,
                    drawCall.m_vertexArray,
                    drawCall.m_transform
                    );
        }

        Renderer::endBatch( ecs.getSystem<CameraSystem>().getViewProjectionMat( p_layerId ) );
    }

    void BasePass::onResize( int p_width, int p_height )
    {
        m_buffer->resize( p_width, p_height );
    }
}
