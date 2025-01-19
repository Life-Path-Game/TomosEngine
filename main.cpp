#include <iostream>

#include "src/Tomos/Util/Logger/Logger.hh"
#include "src/Tomos/Core/Application/Application.hh"
//uwu
using namespace Tomos;

int main()
{
    Logger::setBuildType( BuildType::DEBUG );

    Application::app().run();

    return 0;
}
