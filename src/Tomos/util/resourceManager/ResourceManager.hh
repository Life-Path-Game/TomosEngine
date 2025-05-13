#pragma once

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace Tomos
{
    class ResourceManager
    {
    public:
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
    };
} // namespace Tomos
