#pragma once

#include <vector>
#include <string>

#include <GL/glew.h>

namespace Tomos
{
    enum class ShaderDataType
    {
        None = 0,
        Float,
        Float2,
        Float3,
        Float4,
        Mat3,
        Mat4,
        Int,
        Int2,
        Int3,
        Int4,
        Bool
    };

    static GLenum shaderDataTypeToOpenGLBaseType( ShaderDataType p_type )
    {
        switch ( p_type )
        {
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4:
                return GL_FLOAT;
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
                return GL_INT;
            case ShaderDataType::Bool:
                return GL_BOOL;
            case ShaderDataType::None:
            default:
                return 0;
        }
    }

    static unsigned int shaderDataTypeSize( ShaderDataType p_type )
    {
        switch ( p_type )
        {
            case ShaderDataType::Float:
                return 4;
            case ShaderDataType::Float2:
                return 4 * 2;
            case ShaderDataType::Float3:
                return 4 * 3;
            case ShaderDataType::Float4:
                return 4 * 4;
            case ShaderDataType::Mat3:
                return 4 * 3 * 3;
            case ShaderDataType::Mat4:
                return 4 * 4 * 4;
            case ShaderDataType::Int:
                return 4;
            case ShaderDataType::Int2:
                return 4 * 2;
            case ShaderDataType::Int3:
                return 4 * 3;
            case ShaderDataType::Int4:
                return 4 * 4;
            case ShaderDataType::Bool:
                return 1;
            case ShaderDataType::None:
            default:
                return 0;
        }
    }

    class BufferElement
    {
    public:
        std::string    m_name;
        unsigned int   m_offset;
        unsigned int   m_size;
        ShaderDataType m_type;
        bool           m_normalized;

        BufferElement() = default;

        BufferElement( ShaderDataType p_type, const std::string& p_name, bool p_normalized = false ) :
            m_name( p_name ),
            m_offset( 0 ),
            m_size( shaderDataTypeSize( p_type ) ),
            m_type( p_type ),
            m_normalized( p_normalized )
        {
        }

        unsigned int getComponentCount() const
        {
            switch ( m_type )
            {
                case ShaderDataType::Float:
                    return 1;
                case ShaderDataType::Float2:
                    return 2;
                case ShaderDataType::Float3:
                    return 3;
                case ShaderDataType::Float4:
                    return 4;
                case ShaderDataType::Mat3:
                    return 3 * 3;
                case ShaderDataType::Mat4:
                    return 4 * 4;
                case ShaderDataType::Int:
                    return 1;
                case ShaderDataType::Int2:
                    return 2;
                case ShaderDataType::Int3:
                    return 3;
                case ShaderDataType::Int4:
                    return 4;
                case ShaderDataType::Bool:
                    return 1;
                case ShaderDataType::None:
                default:
                    return 0;
            }
        }
    };

    class BufferLayout
    {
    public:
        BufferLayout() = default;

        BufferLayout( const std::initializer_list<BufferElement>& p_elements ) :
            m_elements( p_elements )
        {
            calculateOffsetsAndStride();
        }

        inline const std::vector<BufferElement>& getElements() const { return m_elements; }
        inline unsigned int                      getStride() const { return m_stride; }

        std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
        std::vector<BufferElement>::iterator end() { return m_elements.end(); }
    private:
        void calculateOffsetsAndStride();

        std::vector<BufferElement> m_elements;
        unsigned int               m_stride = 0;
    };

    class VertexBuffer
    {
    public:
        void bind() const;
        void unbind() const;

        void                setLayout( const BufferLayout& p_layout ) { m_layout = p_layout; }
        const BufferLayout& getLayout() const { return m_layout; }
        unsigned int        getSize() const { return m_size; } // Add this line

        VertexBuffer( const float* p_verticies, unsigned int p_size, GLenum p_usage = GL_STATIC_DRAW );
        ~VertexBuffer();

    private:
        unsigned int m_rendererId{};
        BufferLayout m_layout;
        unsigned int m_size{};
    };

    class IndexBuffer
    {
    public:
        void bind() const;
        void unbind() const;

        IndexBuffer( const unsigned int* p_indicies, unsigned int p_count, GLenum p_usage = GL_STATIC_DRAW );
        ~IndexBuffer();

        unsigned int getCount() const { return m_count; }

    private:
        unsigned int m_rendererId{};
        unsigned int m_count;
    };
} // Tomos
