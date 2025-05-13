#pragma once
#include <memory>

#include "Ecs.hh"
#include "Layer.hh"
#include "Tomos/util/input/Input.hh"
#include "Tomos/util/time/Time.hh"
#include "Window.hh"
#include "Tomos/util/conf/Config.hh"

namespace Tomos
{
    class WindowCloseEvent;
    class Application;

    struct State
    {
    public:
        State() = default;

        ECS&           ecs() { return m_ecs; }
        LayerStack&    layerStack() { return m_layerStack; }
        Input&         input() { return m_input; }
        Time&          time() { return m_time; }
        ConfigManager& config() { return m_config; }

    private:
        ECS m_ecs;

        LayerStack m_layerStack;

        Input m_input;
        Time  m_time;

        ConfigManager m_config;
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

        static State& getState()
        {
            LOG_ASSERT_MSG( g_instance,
                            "Application is not initialized!" );

            return g_instance->m_state;
        }

        void pushLayer( Layer* p_layer );
        void pushOverlay( Layer* p_overlay );

    private:
        Application( const WindowProps& p_props = WindowProps() );

        ~Application()
        {
            delete g_instance;
        };

        Application( const Application& )            = delete;
        Application& operator=( const Application& ) = delete;

        std::unique_ptr<Window> m_window{};

        bool m_running = true;

        static Application* g_instance;
        State               m_state;
    };
} // namespace Tomos
