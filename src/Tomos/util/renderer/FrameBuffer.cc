//
// Created by dstuden on 5/10/25.
//

#include "FrameBuffer.hh"
#include "Tomos/util/logger/Logger.hh"

namespace Tomos
{
    FrameBuffer::FrameBuffer( unsigned int  width, unsigned int        height,
                              TextureFormat colorFormat, TextureFormat depthFormat ) :
        m_size( width, height ), m_colorFormat( colorFormat ), m_depthFormat( depthFormat )
    {
        initialize();
    }

    FrameBuffer::~FrameBuffer()
    {
        cleanup();
    }

    void FrameBuffer::initialize()
    {
        // Create and bind framebuffer
        glGenFramebuffers( 1, &m_fbo );
        glBindFramebuffer( GL_FRAMEBUFFER, m_fbo );

        // Create color texture attachment
        m_colorTexture = Texture::create( m_colorFormat, m_size );
        m_colorTexture->setFilter( TextureFilter::Linear, TextureFilter::Linear );
        m_colorTexture->setWrap( TextureWrap::ClampToEdge, TextureWrap::ClampToEdge );

        // Attach color texture to framebuffer
        glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                GL_TEXTURE_2D, m_colorTexture->getID(), 0 );

        // Create depth texture attachment
        m_depthTexture = Texture::create( m_depthFormat, m_size );
        m_depthTexture->setFilter( TextureFilter::Nearest, TextureFilter::Nearest );
        m_depthTexture->setWrap( TextureWrap::ClampToEdge, TextureWrap::ClampToEdge );

        // Attach depth texture to framebuffer
        glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                GL_TEXTURE_2D, m_depthTexture->getID(), 0 );

        // Check completeness
        if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
        {
            LOG_ERROR() << "Framebuffer is not complete!";
            cleanup();
        }

        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    }

    void FrameBuffer::cleanup()
    {
        if ( m_fbo )
        {
            glDeleteFramebuffers( 1, &m_fbo );
            m_fbo = 0;
        }
        if ( m_rbo )
        {
            glDeleteRenderbuffers( 1, &m_rbo );
            m_rbo = 0;
        }
    }

    void FrameBuffer::bind() const
    {
        if ( !isValid() ) return;
        glBindFramebuffer( GL_FRAMEBUFFER, m_fbo );
        glViewport( 0, 0, m_size.x, m_size.y );
    }

    void FrameBuffer::unbind() const
    {
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    }

    void FrameBuffer::resize( unsigned int width, unsigned int height )
    {
        if ( width == m_size.x && height == m_size.y ) return;

        m_size = {width, height};

        // Resize textures
        m_colorTexture->resize( m_size );
        m_depthTexture->resize( m_size );

        // Reinitialize if needed (though texture resize should be sufficient)
        cleanup();
        initialize();
    }
}
