//
// Created by oskar on 1/19/25.
//

#include "Node.hh"

#include <functional>

namespace Tomos
{

    Node::Node()
    {
        this->parent   = nullptr;
        this->children = std::set<std::unique_ptr<Node>>();
    }

    void Node::addChild( std::unique_ptr<Node> child ) { this->children.insert( child ); }

    bool Node::removeChild( std::unique_ptr<Node> child )
    {
        if ( this->children.erase( child ) )
        {
            return true;
        }
        return false;
    }

    void Node::traverse( const std::function<void( Node& )>& before, const std::function<void( Node& )>& after )
    {
        before( *this );
        for ( auto& current : children )
        {
            current->traverse( before, after );
        }
        after( *this );
    }


}  // namespace Tomos
