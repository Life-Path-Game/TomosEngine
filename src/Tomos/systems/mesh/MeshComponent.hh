#pragma once
#include <memory>

#include "Mesh.hh"
#include "Material.hh"
#include "Tomos/systems/Component.hh"

namespace Tomos
{
    class MeshComponent : public Component
    {
    public:
        MeshComponent( const std::shared_ptr<Mesh>& p_mesh, const std::shared_ptr<Material>& p_material, const std::string& p_name = "MeshComponent" )
        {
            m_material = p_material;
            m_mesh     = p_mesh;
            m_name     = p_name;
        }

        std::shared_ptr<Mesh>     getMesh() const { return m_mesh; }
        std::shared_ptr<Material> getMaterial() const { return m_material; }

        void setMesh( const std::shared_ptr<Mesh>& p_mesh )
        {
            m_mesh = p_mesh;
        }

        void setMaterial( const std::shared_ptr<Material>& p_material )
        {
            m_material = p_material;
        }

    private:
        std::shared_ptr<Mesh>     m_mesh;
        std::shared_ptr<Material> m_material;
    };
} // Tomos
