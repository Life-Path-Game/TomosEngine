//
// Created by dstuden on 1/19/25.
//
#include "Window.hh"

#include "../../Events/Application/ApplicationEvent.hh"
#include "../../Events/Key/KeyEvent.hh"
#include "../../Events/Mouse/MouseEvent.hh"
#include "../../Util/Logger/Logger.hh"


namespace Tomos
{

    Window::Window( const WindowProps& props )
    {
        data.title  = props.title;
        data.width  = props.width;
        data.height = props.height;

        Logger::log() << "Creating window " << data.title << " (" << data.width << ", " << data.height << ")";

        if ( !glfwInit() )
        {
            Logger::log( LogLevel::ERROR ) << "Failed to initialize GLFW!";
            exit( EXIT_FAILURE );
        }

        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

        window = glfwCreateWindow( data.width, data.height, data.title.c_str(), nullptr, nullptr );
        glfwMakeContextCurrent( window );
        glfwSetWindowUserPointer( window, &data );

        glewExperimental = GL_TRUE;
        if ( glewInit() != GLEW_OK )
        {
            Logger::log( LogLevel::ERROR ) << "Failed to initialize GLEW!";
            exit( EXIT_FAILURE );
        }

        Logger::log() << "OpenGL Version: " << glGetString( GL_VERSION );

        glfwSetErrorCallback( []( int error, const char* description )
                              { Logger::log( LogLevel::ERROR ) << "GLFW Error (" << error << "): " << description; } );

        glfwSetWindowSizeCallback( window,
                                   []( GLFWwindow* window, int width, int height )
                                   {
                                       WindowData& data = *( WindowData* ) glfwGetWindowUserPointer( window );
                                       data.width       = width;
                                       data.height      = height;

                                       WindowResizeEvent event( width, height );
                                       data.eventCallback( event );
                                   } );

        glfwSetWindowCloseCallback( window,
                                    []( GLFWwindow* window )
                                    {
                                        WindowData&      data = *( WindowData* ) glfwGetWindowUserPointer( window );
                                        WindowCloseEvent event;
                                        data.eventCallback( event );
                                    } );

        glfwSetKeyCallback( window,
                            []( GLFWwindow* window, int key, int scancode, int action, int mods )
                            {
                                WindowData& data = *( WindowData* ) glfwGetWindowUserPointer( window );

                                switch ( action )
                                {
                                    case GLFW_PRESS:
                                    {
                                        KeyPressedEvent event( key, 0 );
                                        data.eventCallback( event );
                                        break;
                                    }
                                    case GLFW_RELEASE:
                                    {
                                        KeyReleasedEvent event( key );
                                        data.eventCallback( event );
                                        break;
                                    }
                                    case GLFW_REPEAT:
                                    {
                                        KeyPressedEvent event( key, 1 );
                                        data.eventCallback( event );
                                        break;
                                    }
                                }
                            } );

        glfwSetMouseButtonCallback( window,
                                    []( GLFWwindow* window, int button, int action, int mods )
                                    {
                                        WindowData& data = *( WindowData* ) glfwGetWindowUserPointer( window );

                                        switch ( action )
                                        {
                                            case GLFW_PRESS:
                                            {
                                                MouseButtonPressedEvent event( button );
                                                data.eventCallback( event );
                                                break;
                                            }
                                            case GLFW_RELEASE:
                                            {
                                                MouseButtonReleasedEvent event( button );
                                                data.eventCallback( event );
                                                break;
                                            }
                                        }
                                    } );

        glfwSetScrollCallback( window,
                               []( GLFWwindow* window, double xOffset, double yOffset )
                               {
                                   WindowData& data = *( WindowData* ) glfwGetWindowUserPointer( window );

                                   MouseScrolledEvent event( xOffset, yOffset );
                                   data.eventCallback( event );
                               } );

        glfwSetCursorPosCallback( window,
                                  []( GLFWwindow* window, double xPos, double yPos )
                                  {
                                      WindowData& data = *( WindowData* ) glfwGetWindowUserPointer( window );

                                      MouseMovedEvent event( xPos, yPos );
                                      data.eventCallback( event );
                                  } );
    }

    Window::~Window() { shutdown(); }

    void Window::shutdown()
    {
        // We only support one window for now
        glfwDestroyWindow( window );
        glfwTerminate();
    }


    void Window::onUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers( window );
    }

    unsigned int Window::getWidth() const { return data.width; }

    unsigned int Window::getHeight() const { return data.height; }

    void Window::setEventCallback( const EventCallback& callback ) { data.eventCallback = callback; }


}  // namespace Tomos
