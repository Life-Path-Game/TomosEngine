#pragma once

#include "Tomos/util/renderer/Texture.hh"
#include "Tomos/systems/mesh/Material.hh"
#include <vector>
#include <memory>
#include "Tomos/lib/cgltf.h"
#include "Tomos/util/resourceManager/ResourceManager.hh"
#include "Tomos/core/Node.hh"

namespace Tomos
{
    class GLBLoader
    {
    public:
        struct LoadResult
        {
            std::shared_ptr<Node>                  rootNode;
            std::vector<std::shared_ptr<Material>> materials;
        };

        static LoadResult LoadGLB(const std::string& filepath,
                                 const std::shared_ptr<Shader>& shader);

    private:
        static void processNode(cgltf_node* node,
                               cgltf_data* data,
                               const std::shared_ptr<Node>& parentNode,
                               const std::shared_ptr<Shader>& shader,
                               std::vector<std::shared_ptr<Material>>& materials);

        static void processMesh(cgltf_mesh* mesh,
                               cgltf_data* data,
                               const std::shared_ptr<Node>& node,
                               const std::shared_ptr<Shader>& shader,
                               std::vector<std::shared_ptr<Material>>& materials);

        static std::shared_ptr<Material> loadMaterial(cgltf_material* cgltfMat,
                                                    const std::shared_ptr<Shader>& shader,
                                                    cgltf_data* data);

        static std::shared_ptr<Texture> loadTexture(cgltf_texture* texture, cgltf_data* data);
        static std::shared_ptr<Material> createDefaultMaterial(const std::shared_ptr<Shader>& shader);
    };
} // namespace Tomos