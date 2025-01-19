#pragma once

#include <memory>
#include <string>

#include "../Window/Window.hh"

namespace Tomos
{
    class WindowCloseEvent;

    class Application
    {
    public:
        void run();

        static Application& app();

        void onEvent( Event& e );

        bool onWindowClose( WindowCloseEvent& e );

    private:
        Application();
        ~Application()                               = default;
        Application( const Application& )            = delete;
        Application& operator=( const Application& ) = delete;

        std::unique_ptr<Window> window;

        bool running = true;
    };

}  // namespace Tomos
