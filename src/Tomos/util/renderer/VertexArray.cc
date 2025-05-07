//
// Created by dstuden on 2/20/25.
//

#include "VertexArray.hh"

#include <GL/glew.h>

#include "../logger/Logger.hh"

namespace Tomos
{
    void VertexArray::bind() const
    {
        glBindVertexArray( m_rendererId );
    }

    void VertexArray::unbind() const
    {
        glBindVertexArray( 0 );
    }

    void VertexArray::addVertexBuffer( const std::shared_ptr<VertexBuffer>& p_vertexBuffer )
    {
        LOG_DEBUG() << "Start";

        LOG_ASSERT( p_vertexBuffer->getLayout().getElements().size() );

        bind();
        p_vertexBuffer->bind();

        unsigned int attribIndex = calculateAtributeIndex();
        auto         l           = p_vertexBuffer->getLayout();
        LOG_DEBUG() << "Binding vertex buffer";
        for ( const auto& element : l )
        {
            LOG_DEBUG() << "Element: " << element.m_name << " Offset: " << element.m_offset << " Size: " << element.m_size << " Index: " << attribIndex;
            glEnableVertexAttribArray( attribIndex );
            glVertexAttribPointer( attribIndex
                                   , element.getComponentCount()
                                   , shaderDataTypeToOpenGLBaseType( element.m_type )
                                   , element.m_normalized ? GL_TRUE : GL_FALSE
                                   , l.getStride(),
                                   ( const void* ) element.m_offset
                    );
            attribIndex++;
        }

        m_vertexBuffers.push_back( p_vertexBuffer );

        LOG_DEBUG() << "End";
    }

    void VertexArray::setIndexBuffer( const std::shared_ptr<IndexBuffer>& p_indexBuffer )
    {
        bind();
        p_indexBuffer->bind();

        m_indexBuffer = p_indexBuffer;
    }

    unsigned int VertexArray::calculateAtributeIndex() const
    {
        unsigned int index = 0;
        for ( const auto& buffer : m_vertexBuffers )
        {
            index += buffer->getLayout().getElements().size();
        }
        return index;
    }

    VertexArray::VertexArray()
    {
        glCreateVertexArrays( 1, &m_rendererId );
    }

    VertexArray::~VertexArray()
    {
    }
} // Tomos
