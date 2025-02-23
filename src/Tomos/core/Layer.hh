#pragma once

#include <memory>
#include <string>

#include "Scene.hh"
#include "Tomos/events/Event.hh"

namespace Tomos
{
    class Layer
    {
    public:
        explicit Layer( const std::string& p_name = "Layer" ) :
            m_name( p_name )
        {
        }

        virtual ~Layer() = default;

        virtual void onAttach()
        {
        }

        virtual void onDetach()
        {
        }

        virtual void onUpdate()
        {
            if ( m_sceneManager.activeScene() )
            {
                m_sceneManager.activeScene()->update();
            }
        }

        virtual void onEvent( Event& p_event )
        {
        }

        inline const std::string& getName() const { return m_name; }

        inline SceneManager& getSceneManager() { return m_sceneManager; }

    protected:
        std::string m_name{};

        SceneManager m_sceneManager;
    };

    // She stack on my layer till i Vector
    class LayerStack
    {
    public:
        LayerStack();
        ~LayerStack();

        void        pushLayer( Layer* p_layer );
        void        pushOverlay( Layer* p_overlay );
        void popLayer( Layer* p_layer );
        void popOverlay( Layer* p_overlay );

        std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
        std::vector<Layer*>::iterator end() { return m_layers.end(); }

    private:
        // Don't care, Raw Pointer
        std::vector<Layer*>           m_layers;
        std::vector<Layer*>::iterator m_layerInsert;
    };
} // namespace Tomos
