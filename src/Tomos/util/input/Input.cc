//
// Created by dstuden on 1/21/25.
//

#include "Input.hh"

#include "../../core/Application.hh"

namespace Tomos
{
    bool Input::isKeyDown( int p_keycode )
    {
        auto state = glfwGetKey( Application::get()->getWindow().getNativeWindow(), p_keycode );

        if ( state == GLFW_PRESS || state == GLFW_REPEAT )
        {
            return true;
        }

        return false;
    }

    bool Input::isMouseDown( int p_button )
    {
        auto state = glfwGetMouseButton( Application::get()->getWindow().getNativeWindow(), p_button );

        if ( state == GLFW_PRESS )
        {
            return true;
        }

        return false;
    }

    double Input::getMouseX()
    {
        auto [x, y] = getMousePos();
        return x;
    }

    double Input::getMouseY()
    {
        auto [x, y] = getMousePos();
        return y;
    }

    std::pair<double, double> Input::getMousePos()
    {
        double x, y;
        glfwGetCursorPos( Application::get()->getWindow().getNativeWindow(), &x, &y );

        return {x, y};
    }

    std::pair<double, double> Input::getMouseDelta()
    {
        if ( m_firstMouseMove )
        {
            m_mousePosOld    = getMousePos();
            m_firstMouseMove = false;
        }

        auto current = getMousePos();
        auto delta   = std::make_pair(
                current.first - m_mousePosOld.first,
                current.second - m_mousePosOld.second
                );
        m_mousePosOld = current; // Update for next call

        return delta;
    }
} // namespace Tomos
