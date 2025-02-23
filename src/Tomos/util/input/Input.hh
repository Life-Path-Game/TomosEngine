#pragma once

#include <map>
#include <string>

namespace Tomos
{

    class Input
    {
    public:
        static bool isKeyDown( int p_keycode );

        static bool   isMouseDown( int p_button );
        static double getMouseX();
        static double getMouseY();
        static std::pair<double, double> getMousePos();
    };

}  // namespace Tomos
