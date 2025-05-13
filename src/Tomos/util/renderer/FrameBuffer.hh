// FrameBuffer.hh
#pragma once

#include "Texture.hh"
#include <memory>
#include <glm/glm.hpp>

namespace Tomos
{
    class FrameBuffer
    {
    public:
        FrameBuffer( unsigned int  p_width, unsigned int p_height,
                     TextureFormat p_colorFormat = TextureFormat::RGBA8,
                     TextureFormat p_depthFormat = TextureFormat::Depth24 );
        ~FrameBuffer();

        void bind() const;
        void unbind() const;

        void resize( unsigned int p_width, unsigned int p_height );

        const std::shared_ptr<Texture>& getColorTexture() const { return m_colorTexture; }
        const std::shared_ptr<Texture>& getDepthTexture() const { return m_depthTexture; }
        const glm::uvec2&               getSize() const { return m_size; }

        bool isValid() const { return m_fbo != 0; }

        unsigned int getId() const { return m_fbo; }

    private:
        unsigned int             m_fbo = 0;
        unsigned int             m_rbo = 0;
        std::shared_ptr<Texture> m_colorTexture;
        std::shared_ptr<Texture> m_depthTexture;
        glm::uvec2               m_size;
        TextureFormat            m_colorFormat;
        TextureFormat            m_depthFormat;

        void initialize();
        void cleanup();
    };
}
