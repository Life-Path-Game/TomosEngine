#pragma once

#include <GL/glew.h>
// make sure to include glew before glfw
#include <GLFW/glfw3.h>
#include <functional>
#include <string>

#include "Tomos/events/Event.hh"

namespace Tomos
{
    struct WindowProps
    {
        std::string  m_title{};
        unsigned int m_width;
        unsigned int m_height;

        explicit WindowProps( const std::string& p_title = "Tomos Engine", unsigned int p_width = 1280,
                              unsigned int p_height = 720 ) : m_title( p_title ), m_width( p_width ), m_height( p_height )
        {
        }
    };

    class Window
    {
    public:
        using EventCallback = std::function<void( Event& )>;

        explicit Window( const WindowProps& p_props );
        ~Window();

        void shutdown();

        void onUpdate();

        unsigned int getHeight() const;
        unsigned int getWidth() const;

        void setEventCallback( const EventCallback& p_callback );

        GLFWwindow* getNativeWindow() const;

    private:
        GLFWwindow* m_window{};

        struct WindowData
        {
            std::string  m_title{};
            unsigned int m_width{}, m_height{};

            EventCallback m_eventCallback{};
        } m_data;
    };

}  // namespace Tomos
