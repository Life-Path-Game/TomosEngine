#pragma once

#include <memory>
#include <vector>

#include "Buffer.hh"

namespace Tomos
{
    class VertexArray
    {
        friend class Renderer;

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
        void                                  setInstanceBuffer( const std::shared_ptr<StorageBuffer>& p_instanceBuffer, unsigned int p_bindingPoint );
        const std::shared_ptr<StorageBuffer>& getInstanceBuffer() const { return m_instanceBuffer; }

        unsigned int                               m_vertexBufferIndex = 0;
        std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
        std::shared_ptr<IndexBuffer>               m_indexBuffer;
        std::shared_ptr<StorageBuffer>             m_instanceBuffer;

        unsigned int m_rendererId{};
    };
} // Tomos
