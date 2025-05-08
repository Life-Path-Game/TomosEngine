//
// Created by dstuden on 1/19/25.
//

#include "Application.hh"
#include <glm/glm.hpp>

#include "Tomos/events/application/ApplicationEvent.hh"
#include "Tomos/systems/camera/CameraSystem.hh"
#include "Tomos/util/logger/Logger.hh"
#include "Tomos/util/renderer/Buffer.hh"
#include "Tomos/util/renderer/Renderer.hh"

namespace Tomos
{
    Application* Application::g_instance = nullptr;

    Application::Application()
    {
        LOG_INFO() << "Tomos Engine";
        LOG_INFO() << "By dstuden";
        LOG_INFO() << "Initializing Application";

        m_window = std::make_unique<Window>( WindowProps() );

        m_window->setEventCallback( [this]( Event& e ) { onEvent( e ); } );
    }

    Application* Application::get()
    {
        if ( g_instance == nullptr )
        {
            g_instance = new Application();
        }

        return g_instance;
    }

    void Application::run()
    {
        LOG_DEBUG() << "Start";

        glEnable( GL_DEPTH_TEST );

        while ( m_running )
        {
            auto currentTime = ( float ) glfwGetTime();
            getState().m_time.update( currentTime );

            getState().m_ecs.earlyUpdate();
            getState().m_ecs.update();

            getState().m_ecs.updateFixedTimeStep( getState().m_time.deltaTime() );

            for ( auto& layer : getState().m_layerStack )
            {
                layer->onUpdate();
            }

            getState().m_ecs.lateUpdate();


            m_window->onUpdate();
            return;
        }
        LOG_DEBUG() << "End";
    }

    void Application::onEvent( Event& p_e )
    {
        LOG_DEBUG() << p_e.toString();

        EventDispatcher dispatcher( p_e );
        dispatcher.dispatch<WindowCloseEvent>( [this]( Event& p_event )
        {
            return onWindowClose( dynamic_cast<WindowCloseEvent&>( p_event ) );
        } );

        dispatcher.dispatch<WindowResizeEvent>( [this]( Event& p_event )
        {
            auto& e = dynamic_cast<WindowResizeEvent&>( p_event );
            // getState().m_aspectRatio = ( float ) e.getWidth() / ( float ) e.getHeight();
            getState().m_width  = e.getWidth();
            getState().m_height = e.getHeight();
            return false;
        } );

        for ( auto it = getState().m_layerStack.end(); it != getState().m_layerStack.begin(); )
        {
            ( *--it )->onEvent( p_e );
            if ( p_e.isHandled() )
            {
                break;
            }
        }
    }

    bool Application::onWindowClose( WindowCloseEvent& p_e )
    {
        m_running = false;
        return true;
    }

    Window& Application::getWindow() const { return *m_window; }

    void Application::PushLayer( Layer* p_layer ) { getState().m_layerStack.pushLayer( p_layer ); }

    void Application::PushOverlay( Layer* p_overlay ) { getState().m_layerStack.pushOverlay( p_overlay ); }
} // namespace Tomos
