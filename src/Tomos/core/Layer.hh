#pragma once

#include <memory>
#include <string>

#include "../events/Event.hh"

namespace Tomos
{

    class Layer
    {
    public:
        explicit Layer( const std::string& name = "Layer" ) : name( name ) {}
        virtual ~Layer();

        virtual void onAttach() {}
        virtual void onDetach() {}
        virtual void onUpdate() {}
        virtual void onEvent( Event& event ) {}

        inline const std::string& getName() const { return name; }

    protected:
        std::string name;
    };

    class LayerStack
    {
    public:
        LayerStack();
        ~LayerStack();

        void pushLayer( std::unique_ptr<Layer> layer );
        void pushOverlay( std::unique_ptr<Layer> overlay );
        void popLayer( std::unique_ptr<Layer> layer );
        void popOverlay( std::unique_ptr<Layer> overlay );

        std::vector<std::unique_ptr<Layer>>::iterator begin() { return layers.begin(); }
        std::vector<std::unique_ptr<Layer>>::iterator end() { return layers.end(); }

    private:
        std::vector<std::unique_ptr<Layer>>           layers;
        std::vector<std::unique_ptr<Layer>>::iterator layerInsert;
    };

}  // namespace Tomos
