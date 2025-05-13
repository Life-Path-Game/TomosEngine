#pragma once

#include "Tomos/util/renderer/Buffer.hh"
#include "Tomos/util/renderer/Shader.hh"
#include "Tomos/util/renderer/VertexArray.hh"

namespace Tomos
{
    class Mesh
    {
    public:
        Mesh( const std::shared_ptr<VertexBuffer>& p_position,
              const std::shared_ptr<VertexBuffer>& p_normal,
              const std::shared_ptr<VertexBuffer>& p_texCoord,
              const std::shared_ptr<VertexBuffer>& p_tangent,
              const std::shared_ptr<IndexBuffer>&  p_index,
              const std::shared_ptr<Shader>&       p_shader
                );

        ~Mesh() = default;

        const std::shared_ptr<VertexArray>& getVertexArray() const { return m_vertexArray; }
        const std::shared_ptr<IndexBuffer>& getIndexBuffer() const { return m_index; }
        const std::shared_ptr<Shader>&      getShader() const { return m_shader; }

    private:
        void setupVertexAttributes() const;

        std::shared_ptr<VertexArray>  m_vertexArray;
        std::shared_ptr<VertexBuffer> m_position;
        std::shared_ptr<VertexBuffer> m_normal;
        std::shared_ptr<VertexBuffer> m_texCoord;
        std::shared_ptr<VertexBuffer> m_tangent;
        std::shared_ptr<IndexBuffer>  m_index;
        std::shared_ptr<Shader>       m_shader;
    };
} // namespace Tomos
