#pragma once
#include <memory>

#include "Tomos/util/input/Input.hh"
#include "Ecs.hh"
#include "Layer.hh"
#include "Window.hh"
#include "Tomos/util/time/Time.hh"

namespace Tomos
{
    class WindowCloseEvent;
    class Application;

    struct State
    {
        friend class Application;

        State()
        {
            LOG_WARN() << "State constructor";
        }

        ECS        m_ecs;
        LayerStack m_layerStack;
        Input      m_input;
        Time       m_time;

        float m_aspectRatio = 16.0f / 9.0f;
        int   m_width       = 1280;
        int   m_height      = 720;
    };

    class Application
    {
    public:
        static Application* get();

        void run();

        void onEvent( Event& p_e );
        bool onWindowClose( WindowCloseEvent& p_e );

        Window& getWindow() const;

        static State& getState() { return get()->m_state; }

        void PushLayer( Layer* p_layer );
        void PushOverlay( Layer* p_overlay );

    private:
        Application();
        ~Application()                               = default;
        Application( const Application& )            = delete;
        Application& operator=( const Application& ) = delete;

        std::unique_ptr<Window> m_window{};

        bool m_running = true;

        static Application* g_instance;
        State               m_state;
    };
} // namespace Tomos
