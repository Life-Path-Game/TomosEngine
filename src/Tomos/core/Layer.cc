//
// Created by dstuden on 1/22/25.
//

#include "Layer.hh"

namespace Tomos
{

    LayerStack::LayerStack() { layerInsert = layers.begin(); }

    LayerStack::~LayerStack() {}

    void LayerStack::pushLayer( std::unique_ptr<Layer> layer )
    {
        layerInsert = layers.insert( layerInsert, std::move( layer ) );
    }

    void LayerStack::pushOverlay( std::unique_ptr<Layer> overlay ) { layers.push_back( std::move( overlay ) ); }

    void LayerStack::popLayer( std::unique_ptr<Layer> layer )
    {
        auto it = std::find( layers.begin(), layers.end(), layer );
        if ( it != layers.end() )
        {
            layers.erase( it );
            layerInsert--;
        }
    }

    void LayerStack::popOverlay( std::unique_ptr<Layer> overlay )
    {
        auto it = std::find( layers.begin(), layers.end(), overlay );
        if ( it != layers.end() )
        {
            layers.erase( it );
        }
    }

}  // namespace Tomos
