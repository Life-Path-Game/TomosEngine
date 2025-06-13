//
// Created by dstuden on 5/31/25.
//

#include "ResourceManager.hh"

#include "Tomos/util/logger/Logger.hh"

namespace Tomos
{
    ResourceManager::ResourceCache ResourceManager::g_resourceCache = {};

    std::shared_ptr<Mesh> ResourceManager::getMesh( const std::string& p_meshName )
    {
        auto it = g_resourceCache.m_meshCache.find( p_meshName );
        if ( it != g_resourceCache.m_meshCache.end() )
        {
            return it->second;
        }

        LOG_WARN() << "Mesh not cached: " << p_meshName;
        return nullptr;
    }

    std::shared_ptr<Material> ResourceManager::getMaterial( const std::string& p_materialName )
    {
        auto it = g_resourceCache.m_materialCache.find( p_materialName );
        if ( it != g_resourceCache.m_materialCache.end() )
        {
            return it->second;
        }

        LOG_WARN() << "Material not cached: " << p_materialName;
        return nullptr;
    }

    std::shared_ptr<Texture> ResourceManager::getTexture( const std::string& p_textureName )
    {
        auto it = g_resourceCache.m_textureCache.find( p_textureName );
        if ( it != g_resourceCache.m_textureCache.end() )
        {
            return it->second;
        }

        LOG_WARN() << "Texture not cached: " << p_textureName;
        return nullptr;
    }

    std::shared_ptr<Shader> ResourceManager::getShader( const std::string& p_shaderName )
    {
        auto it = g_resourceCache.m_shaderCache.find( p_shaderName );
        if ( it != g_resourceCache.m_shaderCache.end() )
        {
            return it->second;
        }

        LOG_WARN() << "Shader not cached: " << p_shaderName;
        return nullptr;
    }

    void ResourceManager::cacheMesh( const std::string& p_meshName, const std::shared_ptr<Mesh>& p_mesh )
    {
        if ( !p_mesh ) return;

        g_resourceCache.m_meshCache[p_meshName] = p_mesh;
        LOG_INFO() << "Mesh added to cache: " << p_meshName;
    }

    void ResourceManager::cacheMaterial( const std::string& p_materialName, const std::shared_ptr<Material>& p_material )
    {
        if ( !p_material ) return;

        g_resourceCache.m_materialCache[p_materialName] = p_material;
        LOG_INFO() << "Material added to cache: " << p_materialName;
    }

    void ResourceManager::cacheShader( const std::string& p_shaderName, const std::shared_ptr<Shader>& p_shader )
    {
        if ( !p_shader ) return;

        g_resourceCache.m_shaderCache[p_shaderName] = p_shader;
        LOG_INFO() << "Shader added to cache: " << p_shaderName;
    }

    void ResourceManager::cacheTexture( const std::string& p_textureName, const std::shared_ptr<Texture>& p_texture )
    {
        if ( !p_texture ) return;

        g_resourceCache.m_textureCache[p_textureName] = p_texture;
        LOG_INFO() << "Texture added to cache: " << p_textureName;
    }
}
