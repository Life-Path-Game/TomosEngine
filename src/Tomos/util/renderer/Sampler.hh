#pragma once

#include <GL/glew.h>
#include <memory>

namespace Tomos
{
    enum class SamplerFilter
    {
        Nearest,
        Linear,
        NearestMipmapNearest,
        LinearMipmapNearest,
        NearestMipmapLinear,
        LinearMipmapLinear
    };

    enum class SamplerWrap
    {
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder
    };

    class Sampler
    {
    public:
        // Create sampler with default parameters (linear filtering, repeat wrap)
        static std::shared_ptr<Sampler> create();

        static std::shared_ptr<Sampler> create( SamplerFilter p_minFilter,
                                                SamplerFilter p_magFilter,
                                                SamplerWrap   p_wrapS,
                                                SamplerWrap   p_wrapT,
                                                float         p_maxAnisotropy = 1.0f );

        ~Sampler();

        void bind( GLuint p_unit ) const;

        void unbind( GLuint p_unit ) const;

        void setFilter( SamplerFilter p_minFilter, SamplerFilter p_magFilter );

        void setWrap( SamplerWrap p_wrapS, SamplerWrap p_wrapT );

        void setAnisotropy( float p_anisotropy );

        GLuint getID() const { return m_id; }

        static std::shared_ptr<Sampler> createLinearRepeat();
        static std::shared_ptr<Sampler> createNearestRepeat();
        static std::shared_ptr<Sampler> createLinearClamp();
        static std::shared_ptr<Sampler> createNearestClamp();
        static std::shared_ptr<Sampler> createAnisotropicRepeat( float p_anisotropy );

    private:
        Sampler( GLuint p_id );

        GLuint m_id;

        static GLenum toGLFilter( SamplerFilter p_filter );
        static GLenum toGLWrap( SamplerWrap p_wrap );
    };
} // namespace Tomos
