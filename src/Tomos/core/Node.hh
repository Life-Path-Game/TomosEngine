//
// Created by oskar on 1/19/25.
//

#pragma once

#include <functional>
#include <memory>
#include <set>

#include "../systems/Component.hh"
#include "../util/transform/Transform.hh"

namespace Tomos
{
    class Node : public std::enable_shared_from_this<Node>
    {
    public:
        Node( const std::string& name = "UnnamedNode" );
        virtual ~Node() { destroy(); };

        void addChild( const std::shared_ptr<Node>& child );
        bool removeChild( const std::shared_ptr<Node>& child );
        void traverse( const std::function<void( Node& )>& before, const std::function<void( Node& )>& after );

        void addComponent( const std::shared_ptr<Component>& component );
        void removeComponent( const std::shared_ptr<Component>& component );

        bool isInScene() const;

        void destroy();

        // 333 those who know
        std::shared_ptr<Node> findChild( const std::function<bool( Node& )>& predicate,
                                         unsigned int                        maxDepth = 333 ) const;

        template<typename T>
        std::shared_ptr<Component> assertComponent() const;

        template<typename T>
        std::shared_ptr<Component> assertChildComponent() const;

        std::shared_ptr<Node> assertChildByName( const std::string& name, unsigned int maxDepth = 333 ) const;

        static void updateTransforms( Node& node );

        std::string name;
        Transform   transform;

    protected:
        std::weak_ptr<Node>                     parent;
        std::set<std::shared_ptr<Node>>         children;
        std::vector<std::shared_ptr<Component>> components;
    };

    class Scene : public Node
    {
    public:
        Scene( const std::string& name = "UnnamedScene" ) : Node( name ) {}

        void computeTransforms() { updateTransforms( *this ); }
    };

}  // namespace Tomos
