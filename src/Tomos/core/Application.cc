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

    Application::Application( const WindowProps& p_props )
    {
        LOG_INFO() << "Tomos Engine";
        LOG_INFO() << "By dstuden";
        LOG_INFO() << "Initializing Application";

        m_window = std::make_unique<Window>( p_props );

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

    void Application::init( const WindowProps& p_props )
    {
        if ( g_instance == nullptr )
        {
            g_instance = new Application( p_props );
        }
        else
        {
            LOG_ERROR() << "Application already exists!";
        }
    }

    void Application::run()
    {
        LOG_DEBUG() << "Start";

        glEnable( GL_DEPTH_TEST );

        while ( m_running )
        {
            // Time update
            auto currentTime = ( float ) glfwGetTime();
            getState().m_time.update( currentTime );

            // Global ECS update
            getState().m_ecs.earlyUpdate();
            getState().m_ecs.update();

            // Global fixed time step ECS update
            getState().m_ecs.updateFixedTimeStep( getState().m_time.deltaTime() );

            for ( auto& layer : getState().m_layerStack )
            {
                // Layer specific ECS update
                getState().m_ecs.earlyUpdate( layer->getLayerId() );
                getState().m_ecs.update( layer->getLayerId() );

                // Layer specific fixed time step ECS update
                getState().m_ecs.updateFixedTimeStep( getState().m_time.deltaTime(), layer->getLayerId() );

                // Layer specific update
                layer->onUpdate();

                // Layer specific late ECS update
                getState().m_ecs.lateUpdate( layer->getLayerId() );
            }

            // Global ECS late update
            getState().m_ecs.lateUpdate();

            // Window update
            m_window->onUpdate();
        }
        LOG_DEBUG() << "End";
    }

    void Application::onEvent( Event& p_e )
    {
        LOG_DEBUG() << p_e.toString();

        EventDispatcher dispatcher( p_e );
        dispatcher.dispatch<WindowCloseEvent>( [this]( Event& p_event ) { return onWindowClose( dynamic_cast<WindowCloseEvent&>( p_event ) ); } );

        dispatcher.dispatch<WindowResizeEvent>(
                [this]( Event& p_event )
                {
                    auto& e = dynamic_cast<WindowResizeEvent&>( p_event );
                    //                    getWindow().getData().m_aspectRatio = ( float ) e.getWidth() / ( float ) e.getHeight();
                    //                    getWindow().getData().m_width       = e.getWidth();
                    //                    getWindow().getData().m_height      = e.getHeight();
                    int viewportWidth, viewportHeight;
                    int offsetX = 0, offsetY = 0;

                    if ( ( float ) e.getWidth() / ( float ) e.getHeight() > getWindow().getData().m_aspectRatio )
                    {
                        // Window is wider than target - add side bars
                        viewportHeight = e.getHeight();
                        viewportWidth  = ( int ) ( e.getHeight() * getWindow().getData().m_aspectRatio );
                        offsetX        = ( e.getWidth() - viewportWidth ) / 2;
                    }
                    else
                    {
                        // Window is taller than target - add top/bottom bars
                        viewportWidth  = e.getWidth();
                        viewportHeight = ( int ) ( e.getWidth() / getWindow().getData().m_aspectRatio );
                        offsetY        = ( e.getHeight() - viewportHeight ) / 2;
                    }

                    // Set the viewport
                    glViewport( offsetX, offsetY, viewportWidth, viewportHeight );
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

    void Application::pushLayer( Layer* p_layer )
    {
        getState().m_layerStack.pushLayer( p_layer );
        p_layer->onAttach();
    }

    void Application::pushOverlay( Layer* p_overlay )
    {
        getState().m_layerStack.pushOverlay( p_overlay );
        p_overlay->onAttach();
    }
}  // namespace Tomos
