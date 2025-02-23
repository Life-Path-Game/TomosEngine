//
// Created by dstuden on 1/22/25.
//

#include "Layer.hh"

#include "Tomos/util/logger/Logger.hh"

namespace Tomos
{
    LayerStack::LayerStack() { m_layerInsert = m_layers.begin(); }

    LayerStack::~LayerStack()
    {
        for ( Layer* layer : m_layers )
        {
            delete layer;
        }
    }

    void LayerStack::pushLayer( Layer* p_layer )
    {
        m_layerInsert = m_layers.insert( m_layerInsert, p_layer );
    }

    void LayerStack::pushOverlay( Layer* p_overlay ) { m_layers.push_back( p_overlay ); }

    void LayerStack::popLayer( Layer* p_layer )
    {
        LOG_DEBUG() << "Start";
        auto it = std::find( m_layers.begin(), m_layers.end(), p_layer );
        if ( it != m_layers.end() )
        {
            m_layers.erase( it );
            m_layerInsert--;
        }
        LOG_DEBUG() << "End";
    }

    void LayerStack::popOverlay( Layer* p_overlay )
    {
        LOG_DEBUG() << "Start";
        auto it = std::find( m_layers.begin(), m_layers.end(), p_overlay );
        if ( it != m_layers.end() )
        {
            m_layers.erase( it );
        }
        LOG_DEBUG() << "End";
    }
} // namespace Tomos
