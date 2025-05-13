//
// Created by dstuden on 2/19/25.
//

#include "GL/glew.h"
#include "Buffer.hh"
#include "../logger/Logger.hh"

namespace Tomos
{
    void BufferLayout::calculateOffsetsAndStride()
    {
        LOG_DEBUG() << "Start";

        unsigned int offset = 0;
        m_stride            = 0;
        for ( auto& element : m_elements )
        {
            element.m_offset  = offset;
            unsigned int size = shaderDataTypeSize( element.m_type );
            offset += size;
            m_stride += size;
            element.m_size = size;

            LOG_DEBUG() << "Element: " << element.m_name << " Offset: " << element.m_offset << " Size: " << element.m_size;
        }
        LOG_DEBUG() << "Stride: " << m_stride;

        LOG_DEBUG() << "End";
    }

    VertexBuffer::VertexBuffer( const float* p_verticies, unsigned int p_size, GLenum p_usage )
    {
        glCreateBuffers( 1, &m_rendererId );
        bind();
        glBufferData( GL_ARRAY_BUFFER, p_size, p_verticies, p_usage );
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers( 1, &m_rendererId );
    }

    void VertexBuffer::bind() const
    {
        LOG_DEBUG() << "Binding vertex buffer";
        glBindBuffer( GL_ARRAY_BUFFER, m_rendererId );
    }

    void VertexBuffer::unbind() const
    {
        LOG_DEBUG() << "Unbinding vertex buffer";
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
    }

    IndexBuffer::IndexBuffer( const unsigned int* p_indicies, unsigned int p_count, GLenum p_usage ) :
        m_count( p_count )
    {
        glCreateBuffers( 1, &m_rendererId );
        // Umm this is real because OpenGL is stupid L
        // I hate state machines
        // I hate state machines
        // I hate state machines
        // I hate state machines
        glBindBuffer( GL_ARRAY_BUFFER, m_rendererId );
        glBufferData( GL_ARRAY_BUFFER, p_count * sizeof( unsigned int ), p_indicies, p_usage );
        m_count = p_count;
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers( 1, &m_rendererId );
    }

    void IndexBuffer::bind() const
    {
        LOG_DEBUG() << "Binding index buffer";
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_rendererId );
    }

    void IndexBuffer::unbind() const
    {
        LOG_DEBUG() << "Unbinding index buffer";
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    }

    StorageBuffer::StorageBuffer( unsigned int p_size, GLenum p_usage ) :
        m_size( p_size )
    {
        glCreateBuffers( 1, &m_rendererId );
        glNamedBufferData( m_rendererId, p_size, nullptr, p_usage );
    }

    StorageBuffer::~StorageBuffer()
    {
        glDeleteBuffers( 1, &m_rendererId );
    }

    void StorageBuffer::bind() const
    {
        glBindBuffer( GL_SHADER_STORAGE_BUFFER, m_rendererId );
    }

    void StorageBuffer::unbind() const
    {
        glBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 );
    }

    void StorageBuffer::bindBase( unsigned int p_bindingPoint ) const
    {
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, p_bindingPoint, m_rendererId );
    }

    void StorageBuffer::setData( const void* p_data, unsigned int p_size, unsigned int p_offset )
    {
        glNamedBufferSubData( m_rendererId, p_offset, p_size, p_data );
    }

    void* StorageBuffer::map( GLenum p_access )
    {
        return glMapNamedBuffer( m_rendererId, p_access );
    }

    void StorageBuffer::unmap()
    {
        glUnmapNamedBuffer( m_rendererId );
    }
} // Tomos
