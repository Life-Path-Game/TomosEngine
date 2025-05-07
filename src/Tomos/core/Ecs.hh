#pragma once

#include <memory>
#include <unordered_map>

#include "Tomos/systems/System.hh"
#include "Tomos/util/logger/Logger.hh"
#include "Node.hh"

namespace Tomos
{
    class ECS
    {
    public:
        template<typename T>
        void registerSystem()
        {
            bool isBase = std::is_base_of_v<System, T>;
            LOG_ASSERT( isBase );

            m_systems[typeid( T )] = std::make_unique<T>();
            LOG_INFO() << "System registered: " << typeid( T ).name();
        }

        template<typename T>
        void registerFixedTimeStepSystem( float p_fixedTimeStep = 0.01f )
        {
            bool isBase = std::is_base_of_v<FixedTimeStepSystem, T>;
            LOG_ASSERT( isBase );

            m_fixedTimeStepSystems[typeid( T )] = std::make_unique<T>( p_fixedTimeStep );
            LOG_INFO() << "Fixed time step system registered: " << typeid( T ).name();
        }

        void registerComponent( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node );
        void destroyComponent( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node );

        void earlyUpdate();
        void update();
        void lateUpdate();

        void updateFixedTimeStep( float p_deltaTime );

        template<typename T>
        T& getSystem()
        {
            std::type_index type = typeid( T );

            if ( m_systems.find( type ) == m_systems.end() )
            {
                LOG_ERROR() << "System not found: " << type.name();
            }
            return *static_cast<T*>( m_systems[type].get() );
        }

        template<typename T>
        T& getFixedTimeStepSystem()
        {
            std::type_index type = typeid( T );

            if ( m_fixedTimeStepSystems.find( type ) == m_fixedTimeStepSystems.end() )
            {
                LOG_ERROR() << "Fixed time step system not found: " << type.name();
            }
            return *static_cast<T*>( m_fixedTimeStepSystems[type].get() );
        }

    private:
        std::unordered_map<std::type_index, std::unique_ptr<System>>              m_systems;
        std::unordered_map<std::type_index, std::unique_ptr<FixedTimeStepSystem>> m_fixedTimeStepSystems;
    };
} // namespace Tomos
