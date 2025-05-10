#pragma once

#include "Tomos/core/Layer.hh"

namespace Tomos
{

    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer( const std::string& p_name = "ImGuiLayer" );
        ~ImGuiLayer() override;

        void onUpdate() override;
        void onEvent( Event& p_event ) override;

        void onAttach() override;
        void onDetach() override;

    protected:
        float m_time = 0.0f;
    };

}  // namespace Tomos
