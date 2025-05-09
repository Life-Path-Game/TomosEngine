//
// Created by dstuden on 3/5/25.
//

#include "ScriptSystem.hh"

namespace Tomos
{
    void ScriptSystem::componentAdded( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node )
    {
        auto sc = std::dynamic_pointer_cast<ScriptComponent>( p_component );
        if ( !m_components.erase( p_component ) )
        {
            LOG_DEBUG() << "Component moved to new node: " << p_component->m_name;
        }
        m_components[p_component] = p_node;
        sc->getScript()->m_node   = p_node;
        sc->getScript()->onAttach();
    }

    void ScriptSystem::componentRemoved( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node )
    {
        auto sc = std::dynamic_pointer_cast<ScriptComponent>( p_component );
        m_components.erase( p_component );
        sc->getScript()->onDetach();
    }

    void ScriptSystem::earlyUpdate()
    {
        for ( auto& [component, node] : m_components )
        {
            if ( !node->isActive() ) continue;

            auto script = std::dynamic_pointer_cast<ScriptComponent>( component )->getScript();
            script->earlyUpdate();
        }
    }

    void ScriptSystem::update()
    {
        for ( auto& [component, node] : m_components )
        {
            auto script = std::dynamic_pointer_cast<ScriptComponent>( component )->getScript();
            script->update();
        }
    }

    void ScriptSystem::lateUpdate()
    {
        for ( auto& [component, node] : m_components )
        {
            auto script = std::dynamic_pointer_cast<ScriptComponent>( component )->getScript();
            script->lateUpdate();
        }
    }
} // Tomos
