//
// Created by dstuden on 2/20/25.
//

#include "VertexArray.hh"

#include <GL/glew.h>
#include <glm/fwd.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

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

        LOG_ASSERT( !p_vertexBuffer->getLayout().getElements().empty() );

        bind();
        p_vertexBuffer->bind();

        auto layout = p_vertexBuffer->getLayout();
        for ( const auto& element : layout )
        {
            switch ( element.m_type )
            {
                case ShaderDataType::Float:
                case ShaderDataType::Float2:
                case ShaderDataType::Float3:
                case ShaderDataType::Float4:
                {
                    glEnableVertexAttribArray( m_vertexBufferIndex );
                    glVertexAttribPointer( m_vertexBufferIndex,
                                           element.getComponentCount(),
                                           shaderDataTypeToOpenGLBaseType( element.m_type ),
                                           element.m_normalized ? GL_TRUE : GL_FALSE,
                                           layout.getStride(),
                                           ( const void* ) element.m_offset );

                    LOG_DEBUG() << "Vertex buffer index: " << m_vertexBufferIndex
                            << " Element: " << element.m_name
                            << " Offset: " << element.m_offset
                            << " Size: " << element.m_size
                            << " Stride: " << layout.getStride()
                            << " Type: " << shaderDataTypeToOpenGLBaseType( element.m_type );
                    m_vertexBufferIndex++;
                    break;
                }
                case ShaderDataType::Int:
                case ShaderDataType::Int2:
                case ShaderDataType::Int3:
                case ShaderDataType::Int4:
                case ShaderDataType::Bool:
                {
                    glEnableVertexAttribArray( m_vertexBufferIndex );
                    glVertexAttribIPointer( m_vertexBufferIndex,
                                            element.getComponentCount(),
                                            shaderDataTypeToOpenGLBaseType( element.m_type ),
                                            layout.getStride(),
                                            ( const void* ) element.m_offset );

                    LOG_DEBUG() << "Vertex buffer index: " << m_vertexBufferIndex
                            << " Element: " << element.m_name
                            << " Offset: " << element.m_offset
                            << " Size: " << element.m_size
                            << " Stride: " << layout.getStride()
                            << " Type: " << shaderDataTypeToOpenGLBaseType( element.m_type );
                    m_vertexBufferIndex++;
                    break;
                }
                case ShaderDataType::Mat3:
                case ShaderDataType::Mat4:
                {
                    uint8_t count = element.getComponentCount();
                    for ( uint8_t i = 0; i < count; i++ )
                    {
                        glEnableVertexAttribArray( m_vertexBufferIndex );
                        glVertexAttribPointer( m_vertexBufferIndex,
                                               count,
                                               shaderDataTypeToOpenGLBaseType( element.m_type ),
                                               element.m_normalized ? GL_TRUE : GL_FALSE,
                                               layout.getStride(),
                                               ( const void* ) ( element.m_offset + sizeof( float ) * count * i ) );
                        glVertexAttribDivisor( m_vertexBufferIndex, 1 );

                        LOG_DEBUG() << "Vertex buffer index: " << m_vertexBufferIndex
                                << " Element: " << element.m_name
                                << " Offset: " << element.m_offset
                                << " Size: " << element.m_size
                                << " Stride: " << layout.getStride()
                                << " Type: " << shaderDataTypeToOpenGLBaseType( element.m_type );
                        m_vertexBufferIndex++;
                    }
                    break;
                }
                default:
                    LOG_ASSERT_MSG( false, "Unknown ShaderDataType!" );
            }
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

    VertexArray::VertexArray()
    {
        glCreateVertexArrays( 1, &m_rendererId );
    }

    VertexArray::~VertexArray()
    {
    }

    void VertexArray::setInstanceBuffer( const std::shared_ptr<StorageBuffer>& p_instanceBuffer, unsigned int p_bindingPoint )
    {
        bind();
        p_instanceBuffer->bindBase( p_bindingPoint );
        m_instanceBuffer = p_instanceBuffer;
    }
} // Tomos
