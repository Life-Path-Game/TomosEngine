#pragma once
#include <memory>

#include "Tomos/systems/Component.hh"
#include "Tomos/util/renderer/Material.hh"
#include "Tomos/util/renderer/VertexArray.hh"

namespace Tomos
{
    class BillboardComponent : public Component
    {
    public:
        BillboardComponent(
                const std::shared_ptr<Material>     p_material,
                const std::shared_ptr<Shader>       p_shader,
                const std::shared_ptr<VertexArray>& p_vertexArray,
                const std::string&                  p_name = "Billboard" )
        {
            m_material    = p_material ? p_material : Material::getDefaultWhite();
            m_shader      = p_shader;
            m_vertexArray = p_vertexArray;
            m_name        = p_name;
        }

    private:
        std::shared_ptr<Material>    m_material;
        std::shared_ptr<Shader>      m_shader;
        std::shared_ptr<VertexArray> m_vertexArray;
    };
} // Tomos
