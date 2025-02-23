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
        // Type of system
        template<typename T>
        void registerSystem()
        {
            systems[typeid( T )] = std::make_unique<T>();
        }

        void registerComponent( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node );
        void destroyComponent( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node );

        void earlyUpdate();
        void update();
        void lateUpdate();

        template<typename T>
        T& getSystem()
        {
            std::type_index type = typeid( T );

            if ( systems.find( type ) == systems.end() )
            {
                LOG_ERROR() << "System not found: " << type.name();
            }
            return *static_cast<T*>( systems[type].get() );
        }

    private:
        std::unordered_map<std::type_index, std::unique_ptr<System>> systems;
    };
} // namespace Tomos
