#pragma once

#include <memory>
#include <string>

#include "Scene.hh"
#include "Tomos/events/Event.hh"
#include "Tomos/util/renderer/FrameBuffer.hh"
#include "Tomos/util/renderer/Shader.hh"
#include "Tomos/util/renderer/passes/RenderPass.hh"

namespace Tomos
{
    class Layer
    {
    public:
        explicit Layer( const std::string& p_name = "Layer" );

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

        void onResize( int p_width, int p_height );

        inline const std::string& getName() const { return m_name; }

        inline SceneManager& getSceneManager() { return m_sceneManager; }

        int getLayerId() const { return m_layerId; }

        void                                            addRenderPass( std::unique_ptr<RenderPass> p_pass );
        const std::vector<std::unique_ptr<RenderPass>>& getRenderPasses() const;

        std::shared_ptr<FrameBuffer> getLayerFramebuffer() const { return m_layerFramebuffer; }

        inline const std::shared_ptr<Shader>& getShader() const { return m_shader; }
        void                                  setShader( const std::shared_ptr<Shader>& p_shader ) { m_shader = p_shader; }

        static int getNewLayerId()
        {
            static int id = 1;
            return id++;
        }

    protected:
        std::string m_name{};

        int m_layerId;

        SceneManager m_sceneManager;

        std::vector<std::unique_ptr<RenderPass>> m_passes;
        std::shared_ptr<FrameBuffer>             m_layerFramebuffer;
        std::shared_ptr<Shader>                  m_shader;
    };

    class LayerStack
    {
    public:
        LayerStack();
        ~LayerStack();

        void pushLayer( Layer* p_layer );
        void pushOverlay( Layer* p_overlay );
        void popLayer( Layer* p_layer );
        void popOverlay( Layer* p_overlay );

        std::vector<Layer*>::iterator         begin() { return m_layers.begin(); }
        std::vector<Layer*>::iterator         end() { return m_layers.end(); }
        std::vector<Layer*>::reverse_iterator rbegin() { return m_layers.rbegin(); }
        std::vector<Layer*>::reverse_iterator rend() { return m_layers.rend(); }

        std::vector<Layer*>::const_iterator         begin() const { return m_layers.begin(); }
        std::vector<Layer*>::const_iterator         end() const { return m_layers.end(); }
        std::vector<Layer*>::const_reverse_iterator rbegin() const { return m_layers.rbegin(); }
        std::vector<Layer*>::const_reverse_iterator rend() const { return m_layers.rend(); }

    private:
        std::vector<Layer*> m_layers;
        unsigned int        m_layerInsertIndex = 0;
    };
} // namespace Tomos
