//
// Created by dstuden on 1/21/25.
//

#include "Input.hh"

#include "../../core/Application.hh"

namespace Tomos
{

    bool Input::isKeyDown( int keycode )
    {
        auto state = glfwGetKey( Application::get()->getWindow().getNativeWindow(), keycode );

        if ( state == GLFW_PRESS || state == GLFW_REPEAT )
        {
            return true;
        }

        return false;
    }

    bool Input::isMouseDown( int button )
    {
        auto state = glfwGetMouseButton( Application::get()->getWindow().getNativeWindow(), button );

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

        return { x, y };
    }

}  // namespace Tomos
