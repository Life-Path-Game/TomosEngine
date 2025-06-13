#pragma once

#include <string>
#include <filesystem>
#include <unordered_map>

namespace fs = std::filesystem;

namespace Tomos
{
    class Mesh;
    class Material;
    class Texture;
    class Shader;

    class ResourceManager
    {
    public:
        struct ResourceCache
        {
            std::unordered_map<std::string, std::shared_ptr<Mesh>>        m_meshCache;
            std::unordered_map<std::string, std::shared_ptr<Material>>    m_materialCache;
            std::unordered_map<std::string, std::shared_ptr<Texture>>     m_textureCache;
            std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaderCache;
        };

        static fs::path getShaderPath( const std::string& p_shaderName )
        {
            return getBasePath() / "shaders" / ( p_shaderName );
        }

        static fs::path getTexturePath( const std::string& p_textureName )
        {
            return getBasePath() / "textures" / ( p_textureName );
        }

        static fs::path GetModelPath( const std::string& p_modelName )
        {
            return getBasePath() / "models" / ( p_modelName );
        }

        static fs::path getBasePath() { return "resources"; }

        static unsigned int getNewResourceId()
        {
            static unsigned int id = 0;
            return id++;
        }

        static void clearCache()
        {
            g_resourceCache.m_meshCache.clear();
            g_resourceCache.m_materialCache.clear();
            g_resourceCache.m_textureCache.clear();
            g_resourceCache.m_shaderCache.clear();
        }

        static std::shared_ptr<Mesh>     getMesh( const std::string& p_meshName );
        static std::shared_ptr<Material> getMaterial( const std::string& p_materialName );
        static std::shared_ptr<Texture>  getTexture( const std::string& p_textureName );
        static std::shared_ptr<Shader>   getShader( const std::string& p_shaderName );

        static void cacheMesh( const std::string& p_meshName, const std::shared_ptr<Mesh>& p_mesh );
        static void cacheMaterial( const std::string& p_materialName, const std::shared_ptr<Material>& p_material );
        static void cacheTexture( const std::string& p_textureName, const std::shared_ptr<Texture>& p_texture );
        static void cacheShader( const std::string& p_shaderName, const std::shared_ptr<Shader>& p_shader );

    protected:
        static ResourceCache g_resourceCache;
    };
} // namespace Tomos
