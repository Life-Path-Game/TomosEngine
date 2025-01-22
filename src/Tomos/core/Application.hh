#pragma once

#include <memory>
#include <string>

#include "../util/input/Input.hh"
#include "Ecs.hh"
#include "Layer.hh"
#include "Node.hh"
#include "Window.hh"

namespace Tomos
{
    class WindowCloseEvent;

    struct State
    {
        ECS   ecs   = ECS();
        Scene scene = Scene( "main" );

        Input input = Input();

        unsigned int gameTime;
        unsigned int deltaTime;

        float aspectRatio = 16.0f / 9.0f;
    };

    class Application
    {
    public:
        static Application* get();

        void run();

        void onEvent( Event& e );
        bool onWindowClose( WindowCloseEvent& e );

        Window& getWindow() const;

        State& getState() { return state; }

        void PushLayer( std::unique_ptr<Layer> layer );
        void PushOverlay( std::unique_ptr<Layer> overlay );

    private:
        Application();
        ~Application()                               = default;
        Application( const Application& )            = delete;
        Application& operator=( const Application& ) = delete;

        std::unique_ptr<Window> window;

        bool running = true;

        LayerStack layerStack;

        static Application* instance;
        State               state;
    };

}  // namespace Tomos
