//
// Created by oskar on 1/19/25.
//

#include "Node.hh"

#include <stack>

#include "Application.hh"

namespace Tomos
{

    Node::Node( const std::string& name ) : name( name ) {}

    void Node::addChild( const std::shared_ptr<Node>& child ) { this->children.emplace( child ); }

    bool Node::removeChild( const std::shared_ptr<Node>& child ) { return this->children.erase( child ) > 0; }

    void Node::traverse( const std::function<void( Node& )>& before, const std::function<void( Node& )>& after )
    {
        before( *this );
        for ( auto& current : children )
        {
            current->traverse( before, after );
        }
        after( *this );
    }

    bool Node::isInScene() const
    {
        if ( dynamic_cast<const Scene*>( this ) != nullptr )
        {
            return true;
        }

        if ( parent.lock() == nullptr )
        {
            return false;
        }

        return parent.lock()->isInScene();
    }

    void Node::addComponent( const std::shared_ptr<Component>& component )
    {
        if ( !isInScene() )
        {
            Application::get()->getState().ecs.registerComponent( component, shared_from_this() );
        }
        this->components.push_back( component );
    }

    void Node::removeComponent( const std::shared_ptr<Component>& component )
    {
        Application::get()->getState().ecs.destroyComponent( component, shared_from_this() );
        auto it = std::find( components.begin(), components.end(), component );
        if ( it != components.end() )
        {
            components.erase( it );
        }
    }

    void Node::destroy()
    {
        for ( const auto& component : components )
        {
            removeComponent( component );
        }

        for ( const auto& child : children )
        {
            child->destroy();
        }

        if ( auto p = parent.lock() )
        {
            p->removeChild( shared_from_this() );
        }
    }

    std::shared_ptr<Node> Node::findChild( const std::function<bool( Node& )>& predicate, unsigned int maxDepth ) const
    {
        if ( maxDepth == 0 ) return nullptr;

        for ( const auto& child : children )
        {
            if ( predicate( *child ) )
            {
                return child;
            }

            auto found = child->findChild( predicate, maxDepth - 1 );
            if ( found ) return found;
        }

        return nullptr;
    }


    template<typename T>
    std::shared_ptr<Component> Node::assertComponent() const
    {
        for ( const auto& component : components )
        {
            if ( dynamic_cast<T*>( component.get() ) != nullptr )
            {
                return component;
            }
        }

        return nullptr;
    }

    template<typename T>
    std::shared_ptr<Component> Node::assertChildComponent() const
    {
        for ( const auto& child : children )
        {
            auto component = child->assertComponent<T>();
            if ( component ) return component;
        }

        return nullptr;
    }

    std::shared_ptr<Node> Node::assertChildByName( const std::string& name, unsigned int maxDepth ) const
    {
        return findChild( [name]( Node& node ) { return node.name == name; }, maxDepth );
    }

    void Node::updateTransforms( Node& root )
    {
        std::stack<Node*> stack;
        stack.push( &root );

        while ( !stack.empty() )
        {
            Node* current = stack.top();
            stack.pop();

            current->transform.update();

            for ( auto& child : current->children )
            {
                stack.push( child.get() );
            }
        }
    }


}  // namespace Tomos
