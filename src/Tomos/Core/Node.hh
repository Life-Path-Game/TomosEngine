//
// Created by oskar on 1/19/25.
//

#pragma once
#include <functional>
#include <memory>
#include <set>

namespace Tomos
{

    class Node
    {
    public:
        Node();
        void addChild( std::unique_ptr<Node> child );
        bool removeChild( std::unique_ptr<Node> child );
        void traverse( const std::function<void( Node& )>& before, const std::function<void( Node& )>& after );
        // TODO:

    private:
        std::unique_ptr<Node>           parent;
        std::set<std::unique_ptr<Node>> children;
    };

}  // namespace Tomos
