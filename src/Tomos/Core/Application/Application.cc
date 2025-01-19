//
// Created by dstuden on 1/19/25.
//

#include "Application.hh"

#include "../../Util/Logger/Logger.hh"
#include "../../Events/Application/ApplicationEvent.hh"

namespace Tomos
{
    Application::Application()
    {
        Logger::log() << "Tomos Engine v0.1";
        Logger::log() << "By dstuden";
        Logger::log() << "Initializing Application";

        window = std::make_unique<Window>( WindowProps() );

        window->setEventCallback( std::bind( &Application::onEvent, this, std::placeholders::_1 ) );
    }

    void Application::run()
    {
        while ( running )
        {
            glClearColor( 1, 0, 1, 1 );
            glClear( GL_COLOR_BUFFER_BIT );
            window->onUpdate();
        }
    }

    Application& Application::app()
    {
        static Application instance;
        return instance;
    }

    void Application::onEvent( Event& e )
    {
        EventDispatcher dispatcher( e );
        dispatcher.dispatch<WindowCloseEvent>( [this]( Event& event )
                                               { return onWindowClose( dynamic_cast<WindowCloseEvent&>( event ) ); } );

        Logger::log( LogLevel::DEBUG ) << e.toString();
    }

    bool Application::onWindowClose( WindowCloseEvent& e )
    {
        running = false;
        return true;
    }

}  // namespace Tomos
