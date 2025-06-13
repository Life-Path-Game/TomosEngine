#include "../../systems/mesh/Mesh.hh"

#include "Tomos/util/logger/Logger.hh"
#include "Tomos/util/renderer/Renderer.hh"

namespace Tomos
{
    Mesh::Mesh( const std::shared_ptr<VertexBuffer>& p_position,
                const std::shared_ptr<VertexBuffer>& p_normal,
                const std::shared_ptr<VertexBuffer>& p_texCoord,
                const std::shared_ptr<VertexBuffer>& p_tangent,
                const std::shared_ptr<IndexBuffer>&  p_index,
                const std::shared_ptr<Shader>&       p_shader ) :
        m_position( p_position ),
        m_normal( p_normal ),
        m_texCoord( p_texCoord ),
        m_tangent( p_tangent ),
        m_index( p_index ),
        m_shader( p_shader )
    {
        m_vertexArray = std::make_shared<VertexArray>();

        setupVertexAttributes();

        // Bind index buffer if present
        if ( m_index )
        {
            m_vertexArray->setIndexBuffer( m_index );
        }
    }

    void Mesh::setupVertexAttributes() const
    {
        if ( !m_position )
        {
            LOG_ERROR() << "Mesh requires a position buffer";
            return;
        }

        // Position (location = 0)
        BufferLayout positionLayout = {
                {ShaderDataType::Float3, "aPosition"}
        };
        m_position->setLayout( positionLayout );
        m_vertexArray->addVertexBuffer( m_position );

        // Normal (location = 1) - make optional
        if ( m_normal )
        {
            BufferLayout normalLayout = {
                    {ShaderDataType::Float3, "aNormal"}
            };
            m_normal->setLayout( normalLayout );
            m_vertexArray->addVertexBuffer( m_normal );
        }
        else
        {
            LOG_WARN() << "Mesh has no normal buffer - using default";
        }

        // Texture Coordinates (location = 2) - make optional
        if ( m_texCoord )
        {
            BufferLayout texCoordLayout = {
                    {ShaderDataType::Float2, "aTexCoord"}
            };
            m_texCoord->setLayout( texCoordLayout );
            m_vertexArray->addVertexBuffer( m_texCoord );
        }
        else
        {
            LOG_WARN() << "Mesh has no texture coordinate buffer - using default";
        }

        // Tangent (location = 3) - optional
        if ( m_tangent )
        {
            BufferLayout tangentLayout = {
                    {ShaderDataType::Float4, "aTangent"}
            };
            m_tangent->setLayout( tangentLayout );
            m_vertexArray->addVertexBuffer( m_tangent );
        }
    }
} // namespace Tomos
