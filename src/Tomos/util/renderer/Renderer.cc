#include "Renderer.hh"
#include "Tomos/util/logger/Logger.hh"

namespace Tomos
{
    std::unordered_map<MaterialBatchKey, 
                      std::unordered_map<std::shared_ptr<VertexArray>, BatchData>, 
                      MaterialBatchKeyHash> Renderer::s_batches;
    bool Renderer::s_batching = false;

    void Renderer::setClearedColor(const glm::vec4& p_color) {
        glClearColor(p_color.r, p_color.g, p_color.b, p_color.a);
    }

    void Renderer::clear(unsigned int p_mask) {
        glClear(p_mask);
    }

    void Renderer::onWindowResize(unsigned int p_width, unsigned int p_height) {
        // glViewport(0, 0, p_width, p_height);
    }

    void Renderer::beginBatch() {
        s_batching = true;
        s_batches.clear();
    }

    void Renderer::endBatch(const glm::mat4& viewProjection) {
        if (!s_batching) return;
        
        for (auto& [materialKey, meshBatches] : s_batches) {
            // Bind shader and material once per group
            materialKey.shader->bind();
            materialKey.material->bind();
            materialKey.shader->setMat4("uViewProjection", viewProjection);
            
            for (auto& [vertexArray, batchData] : meshBatches) {
                vertexArray->bind();
                
                // Draw all instances with this shader/material/VAO combination
                for (const auto& transform : batchData.transforms) {
                    materialKey.shader->setMat4("uTransform", transform);
                    
                    glDrawElements(GL_TRIANGLES,
                                 vertexArray->getIndexBuffer()->getCount(),
                                 GL_UNSIGNED_INT,
                                 nullptr);
                }
            }
        }
        
        s_batching = false;
        s_batches.clear();
    }

    void Renderer::addToBatch(const std::shared_ptr<Shader>& p_shader,
                             const std::shared_ptr<Material>& p_material,
                             const std::shared_ptr<VertexArray>& p_vertexArray,
                             const glm::mat4& p_transform) {
        if (!s_batching) return;
        
        MaterialBatchKey key{p_shader, p_material};
        auto& batchData = s_batches[key][p_vertexArray];
        if (!batchData.vertexArray) {
            batchData.vertexArray = p_vertexArray;
        }
        batchData.transforms.push_back(p_transform);
    }

    void Renderer::draw(const std::shared_ptr<Shader>& p_shader,
                       const std::shared_ptr<Material>& p_material,
                       const std::shared_ptr<VertexArray>& p_vertexArray,
                       const glm::mat4& p_transform,
                       const glm::mat4& p_viewProjection) {
        if (s_batching) {
            addToBatch(p_shader, p_material, p_vertexArray, p_transform);
            return;
        }
        
        // Fallback to immediate mode if not batching
        if (!p_vertexArray || !p_vertexArray->getIndexBuffer()) {
            LOG_ERROR() << "Renderer::draw() - Invalid VertexArray or IndexBuffer";
            return;
        }

        p_shader->bind();
        p_material->bind();
        p_shader->setMat4("uTransform", p_transform);
        p_shader->setMat4("uViewProjection", p_viewProjection);

        p_vertexArray->bind();
        glDrawElements(GL_TRIANGLES,
                      p_vertexArray->getIndexBuffer()->getCount(),
                      GL_UNSIGNED_INT,
                      nullptr);

        // Error checking
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            LOG_ERROR() << "OpenGL error in Renderer::draw(): " << err;
        }
    }
}