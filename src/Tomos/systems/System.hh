#pragma once

#include <map>
#include <typeindex>

#include "Tomos/core/Node.hh"
#include "Component.hh"

namespace Tomos
{
    class System
    {
    public:
        virtual ~System() = default;

        virtual void earlyUpdate()
        {
        };

        virtual void update()
        {
        };

        virtual void lateUpdate()
        {
        };

        virtual void componentAdded( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node )
        {
        };

        virtual void componentRemoved( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node )
        {
        };

        virtual std::type_index getComponentType() const { return typeid( nullptr ); };

    protected:
        std::map<std::shared_ptr<Component>, std::shared_ptr<Node>> m_components;
    };

    class FixedTimeStepSystem : public System
    {
    public:
        FixedTimeStepSystem( float p_fixedTimeStep = 0.01f ) :
            m_fixedTimeStep( p_fixedTimeStep )
        {
        }

        virtual void update( float p_deltaTime )
        {
            m_accumulator += p_deltaTime;

            while ( m_accumulator >= m_fixedTimeStep )
            {
                m_accumulator -= m_fixedTimeStep;
                System::earlyUpdate();
                System::update();
                System::lateUpdate();
            }
        }

    protected:
        float m_fixedTimeStep = 0.01; // 100Hz
        float m_accumulator   = 0.0f;
    };
} // namespace Tomos
