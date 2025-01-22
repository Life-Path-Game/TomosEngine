#include <iostream>

#include "src/Tomos/core/Application.hh"
#include "src/Tomos/util/logger/Logger.hh"

using namespace Tomos;

int main()
{
    Application::get()->run();

    return 0;
}
