#pragma once

#include <memory>
#include <vector>

#include "Buffer.hh"

namespace Tomos
{
    class VertexArray
    {
    public:
        void bind() const;
        void unbind() const;

        void addVertexBuffer( const std::shared_ptr<VertexBuffer>& p_vertexBuffer );
        void setIndexBuffer( const std::shared_ptr<IndexBuffer>& p_indexBuffer );

        const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const { return m_vertexBuffers; }
        const std::shared_ptr<IndexBuffer>&               getIndexBuffer() const { return m_indexBuffer; }


        VertexArray();
        ~VertexArray();

    private:
        unsigned int calculateAtributeIndex() const;

        std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
        std::shared_ptr<IndexBuffer>               m_indexBuffer;

        unsigned int m_rendererId{};
    };
} // Tomos
