#include <iostream>

#include "src/Tomos/Core/Application.hh"
#include "src/Tomos/Util/Logger/Logger.hh"
// uwu
using namespace Tomos;

int main()
{
    Logger::setBuildType( BuildType::DEBUG );

    Application::app().run();

    return 0;
}
