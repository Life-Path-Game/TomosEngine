#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>
#include <string>

namespace Tomos
{
    enum class TextureFormat
    {
        R8,
        RG8,
        RGB8,
        RGBA8,
        SRGB8,
        SRGBA8,
        RGB16F,
        RGBA16F,
        Depth16,
        Depth24,
        Depth32F
    };

    enum class TextureFilter
    {
        Nearest,
        Linear,
        NearestMipmapNearest,
        LinearMipmapNearest,
        NearestMipmapLinear,
        LinearMipmapLinear
    };

    enum class TextureWrap
    {
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder
    };

    class Texture
    {
    public:
        static std::shared_ptr<Texture> createWhiteTexture();
        static std::shared_ptr<Texture> createBlackTexture();
        static std::shared_ptr<Texture> createDefaultNormalMap();

        static std::shared_ptr<Texture> createFromFile( const std::string& p_path,
                                                        TextureFormat      p_format          = TextureFormat::SRGBA8,
                                                        bool               p_generateMipmaps = true );

        static std::shared_ptr<Texture> createFromMemory( const unsigned char* p_data,
                                                          size_t               p_length,
                                                          TextureFormat        p_format );

        static std::shared_ptr<Texture> createFromPixels( const unsigned char* p_pixels,
                                                          int                  p_width,
                                                          int                  p_height,
                                                          TextureFormat        p_format
                );

        static std::shared_ptr<Texture> create( TextureFormat     p_format,
                                                const glm::uvec2& p_size,
                                                bool              p_generateMipmaps = false );

        ~Texture();

        void bind( GLuint p_unit ) const;

        static void unbind( GLuint p_unit );

        // Resize texture (contents will be undefined)
        void resize( const glm::uvec2& p_newSize );

        const glm::uvec2& getSize() const { return m_size; }

        GLuint getID() const { return m_id; }

        TextureFormat getFormat() const { return m_format; }

        void generateMipmaps();

        // Set texture parameters
        void setFilter( TextureFilter p_minFilter, TextureFilter p_magFilter ) const;
        void setWrap( TextureWrap p_wrapS, TextureWrap p_wrapT ) const;

        static GLenum toGLFormat( TextureFormat p_format );
        static GLenum toGLInternalFormat( TextureFormat p_format );
        static GLenum toGLDataType( TextureFormat p_format );
        static GLenum toGLFilter( TextureFilter p_filter );
        static GLenum toGLWrap( TextureWrap p_wrap );

    private:
        Texture( GLuint p_id, TextureFormat p_format, const glm::uvec2& p_size );

        GLuint        m_id;
        TextureFormat m_format;
        glm::uvec2    m_size;
        bool          m_hasMipmaps;
    };
} // namespace Tomos
