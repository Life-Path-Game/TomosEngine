#pragma once

#include <GL/glew.h>
// make sure to include glew before glfw
#include <GLFW/glfw3.h>
#include <functional>
#include <string>

#include "../Events/Event.hh"

namespace Tomos
{
    struct WindowProps
    {
        std::string  title;
        unsigned int width;
        unsigned int height;

        WindowProps( const std::string& title = "Tomos Engine", unsigned int width = 1280, unsigned int height = 720 ) :
            title( title ), width( width ), height( height )
        {
        }
    };

    class Window
    {
    public:
        using EventCallback = std::function<void( Event& )>;

        Window( const WindowProps& props );
        ~Window();

        void shutdown();

        void onUpdate();

        unsigned int getHeight() const;
        unsigned int getWidth() const;

        void setEventCallback( const EventCallback& callback );

    private:
        GLFWwindow* window;

        struct WindowData
        {
            std::string  title;
            unsigned int width, height;

            EventCallback eventCallback;
        } data;
    };

}  // namespace Tomos
