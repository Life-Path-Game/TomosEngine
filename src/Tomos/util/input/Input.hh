#pragma once

#include <map>
#include <string>

namespace Tomos
{

    class Input
    {
    public:
        static bool isKeyDown( int keycode );

        static bool   isMouseDown( int button );
        static double getMouseX();
        static double getMouseY();
        static std::pair<double, double> getMousePos();
    };

}  // namespace Tomos
