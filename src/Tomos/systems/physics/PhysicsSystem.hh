#pragma once
#include "Tomos/systems/System.hh"
#include "Jolt/Jolt.h"
#include "Jolt/Physics/PhysicsSystem.h"
#include "Jolt/Core/JobSystemThreadPool.h"

namespace Tomos
{
    class ColliderComponent : public Component
    {
    public:
        std::string m_name = "ColliderComponent";
    };

    /*
     * ! SPECIAL SYSTEM !
     * Can be used on the variable time step or on a separate fixed time step
     * Register it in the ECS either as a normal system or as a fixed time step system
     */
    class PhysicsSystem : public FixedTimeStepSystem
    {
    public:
        PhysicsSystem( float p_fixedTimeStep = 0.01f ) :
            FixedTimeStepSystem( p_fixedTimeStep )
        {
            initJolt();
        }

        std::type_index getComponentType() const override { return typeid( ColliderComponent ); }

        void componentAdded( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node ) override;
        void componentRemoved( const std::shared_ptr<Component>& p_component, const std::shared_ptr<Node>& p_node ) override;

        void update() override;

        void update( float p_deltaTime ) override;

    private:
        void initJolt();

        JPH::PhysicsSystem                        m_physicsSystem;
        std::shared_ptr<JPH::TempAllocatorImpl>   m_tempAllocator;
        std::shared_ptr<JPH::JobSystemThreadPool> m_jobSystem;

        float m_fixedTimeStep   = 0.01f;
        int   m_maxPhysicsSteps = 3;
    };
} // Tomos
