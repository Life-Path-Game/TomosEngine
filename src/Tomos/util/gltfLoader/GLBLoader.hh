#pragma once

#include "Tomos/util/renderer/Texture.hh"
#include "Tomos/systems/mesh/Material.hh"
#include <vector>
#include <memory>
#include "Tomos/lib/cgltf.h"
#include "Tomos/util/resourceManager/ResourceManager.hh"
#include "Tomos/core/Node.hh"
#include "Tomos/systems/mesh/Mesh.hh"

namespace Tomos
{
    class GLBLoader
    {
    public:
        struct LoadResult
        {
            std::shared_ptr<Node>                  m_rootNode;
            std::vector<std::shared_ptr<Material>> m_materials;
        };

        // Resource cache structure
        struct GLBResourceCache
        {
            std::unordered_map<std::string, std::shared_ptr<Mesh>>     m_meshCache;
            std::unordered_map<std::string, std::shared_ptr<Material>> m_materialCache;
            std::unordered_map<std::string, std::shared_ptr<Texture>>  m_textureCache;
        };

        static LoadResult loadGLB( const std::string&             p_filepath,
                                   const std::shared_ptr<Shader>& p_shader,
                                   bool                           p_useCache = true );

        static std::shared_ptr<Node> createInstance( const std::shared_ptr<Node>& p_original );

    private:
        static void processNode( cgltf_node*                             p_node,
                                 cgltf_data*                             p_data,
                                 const std::shared_ptr<Node>&            p_parentNode,
                                 const std::shared_ptr<Shader>&          p_shader,
                                 std::vector<std::shared_ptr<Material>>& p_materials,
                                 bool                                    p_useCache );

        static void processMesh( cgltf_mesh*                             p_mesh,
                                 cgltf_data*                             p_data,
                                 const std::shared_ptr<Node>&            p_node,
                                 const std::shared_ptr<Shader>&          p_shader,
                                 std::vector<std::shared_ptr<Material>>& p_materials,
                                 bool                                    p_useCache );

        static std::shared_ptr<Material> loadMaterial( cgltf_material*                p_cgltfMat,
                                                       const std::shared_ptr<Shader>& p_shader,
                                                       cgltf_data*                    p_data,
                                                       bool                           p_useCache );

        static std::shared_ptr<Texture>  loadTexture( cgltf_texture* p_texture, cgltf_data* p_data, bool p_useCache );
        static std::shared_ptr<Material> createDefaultMaterial( const std::shared_ptr<Shader>& p_shader );

        static std::shared_ptr<Node> deepCopyNode( const std::shared_ptr<Node>& p_original );

        // Clearing the cache does not remove any meshes or materials from the scene graph
        // which are on a node due to them being shared pointers. It only clears the cache of the GLBLoader.
        static void clearCache();
    };
} // namespace Tomos
