#pragma once
#include <memory>

#include "Ecs.hh"
#include "Layer.hh"
#include "Tomos/util/input/Input.hh"
#include "Tomos/util/time/Time.hh"
#include "Window.hh"

namespace Tomos
{
    class WindowCloseEvent;
    class Application;

    struct State
    {
        friend class Application;

        State() { LOG_WARN() << "State constructor"; }

        ECS m_ecs;

        // Not intended to be used directly
        LayerStack m_layerStack;

        Input m_input;
        Time  m_time;
    };

    class Application
    {
    public:
        static Application* get();
        static void         init( const WindowProps& p_props = WindowProps() );

        void run();

        void onEvent( Event& p_e );
        bool onWindowClose( WindowCloseEvent& p_e );

        Window& getWindow() const;

        static State& getState() { return get()->m_state; }

        void pushLayer( Layer* p_layer );
        void pushOverlay( Layer* p_overlay );

    private:
        Application( const WindowProps& p_props = WindowProps() );
        ~Application()                               = default;
        Application( const Application& )            = delete;
        Application& operator=( const Application& ) = delete;

        std::unique_ptr<Window> m_window{};

        bool m_running = true;

        static Application* g_instance;
        State               m_state;
    };
}  // namespace Tomos
