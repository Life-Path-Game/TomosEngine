#pragma once

#include "Tomos/systems/mesh/Mesh.hh"
#include "Tomos/util/renderer/Texture.hh"
#include "Tomos/systems/mesh/Material.hh"
#include <vector>
#include <memory>
#include <assimp/scene.h>
#include "Tomos/util/logger/Logger.hh"
#include "Tomos/util/resourceManager/ResourceManager.hh"

namespace Tomos
{
    class GLBLoader
    {
    public:
        struct LoadResult {
            std::shared_ptr<Mesh> mesh;
            std::shared_ptr<Material> material;
        };

        static LoadResult LoadGLB(const std::string& filepath,
                                const std::shared_ptr<Shader>& shader);

    private:
        static std::shared_ptr<Material> loadMaterial(aiMaterial* aiMat,
                                                    const std::shared_ptr<Shader>& shader,
                                                    const aiScene* scene);

        static std::shared_ptr<Texture> loadEmbeddedTexture(const aiTexture* aiTex);
        static std::shared_ptr<Material> createDefaultMaterial(const std::shared_ptr<Shader>& shader);
    };
} // namespace Tomos