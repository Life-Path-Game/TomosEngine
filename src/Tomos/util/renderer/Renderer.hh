#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>
#include "Shader.hh"
#include "VertexArray.hh"
#include "Tomos/systems/mesh/Material.hh"

namespace Tomos
{
    struct BatchData {
        std::shared_ptr<VertexArray> vertexArray;
        std::vector<glm::mat4> transforms;
    };

    struct MaterialBatchKey {
        std::shared_ptr<Shader> shader;
        std::shared_ptr<Material> material;

        bool operator==(const MaterialBatchKey& other) const {
            return shader == other.shader && material == other.material;
        }
    };

    struct MaterialBatchKeyHash {
        size_t operator()(const MaterialBatchKey& key) const {
            return std::hash<std::shared_ptr<Shader>>()(key.shader) ^
                   std::hash<std::shared_ptr<Material>>()(key.material);
        }
    };

    class Renderer
    {
    public:
        static void setClearedColor(const glm::vec4& p_color);
        static void clear(unsigned int p_mask);
        static void onWindowResize(unsigned int p_width, unsigned int p_height);

        // Individual draw call (now uses batching internally)
        static void draw(const std::shared_ptr<Shader>& p_shader,
                        const std::shared_ptr<Material>& p_material,
                        const std::shared_ptr<VertexArray>& p_vertexArray,
                        const glm::mat4& p_transform,
                        const glm::mat4& p_viewProjection);

        // Batch rendering interface
        static void beginBatch();
        static void endBatch(const glm::mat4& viewProjection);
        static void addToBatch(const std::shared_ptr<Shader>& p_shader,
                              const std::shared_ptr<Material>& p_material,
                              const std::shared_ptr<VertexArray>& p_vertexArray,
                              const glm::mat4& p_transform);

    private:
        static std::unordered_map<MaterialBatchKey,
                                std::unordered_map<std::shared_ptr<VertexArray>, BatchData>,
                                MaterialBatchKeyHash> s_batches;
        static bool s_batching;
    };
}