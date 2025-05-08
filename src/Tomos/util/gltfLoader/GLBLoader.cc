#define GLM_ENABLE_EXPERIMENTAL
#define CGLTF_IMPLEMENTATION

#include "GLBLoader.hh"
#include <filesystem>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "Tomos/systems/mesh/MeshComponent.hh"
#include "Tomos/util/logger/Logger.hh"
#include "Tomos/util/renderer/Buffer.hh"

namespace Tomos
{
    GLBLoader::LoadResult GLBLoader::LoadGLB( const std::string&             filepath,
                                              const std::shared_ptr<Shader>& shader )
    {
        LoadResult result;

        // Load GLB file using cgltf
        cgltf_options options      = {};
        cgltf_data*   data         = nullptr;
        cgltf_result  parse_result = cgltf_parse_file( &options, filepath.c_str(), &data );

        if ( parse_result != cgltf_result_success )
        {
            LOG_ERROR() << "Failed to load GLB file: " << filepath << " Error: " << parse_result;
            return result;
        }

        // Load buffers
        cgltf_result load_result = cgltf_load_buffers( &options, data, filepath.c_str() );
        if ( load_result != cgltf_result_success )
        {
            LOG_ERROR() << "Failed to load GLB buffers: " << filepath << " Error: " << load_result;
            cgltf_free( data );
            return result;
        }

        // Create root node
        auto rootNode   = std::make_shared<SceneNode>( "GLB_Root" );
        result.rootNode = rootNode;

        // Process all nodes recursively
        for ( size_t i = 0; i < data->nodes_count; i++ )
        {
            if ( !data->nodes[i].parent ) // Only process root nodes, children will be processed recursively
            {
                processNode( &data->nodes[i], data, rootNode, shader, result.materials );
            }
        }

        // Free cgltf data
        cgltf_free( data );

        return result;
    }

    void GLBLoader::processNode( cgltf_node*                             node, cgltf_data* data,
                                 const std::shared_ptr<Node>&            parentNode,
                                 const std::shared_ptr<Shader>&          shader,
                                 std::vector<std::shared_ptr<Material>>& materials )
    {
        // Create a new node
        auto newNode = std::make_shared<SceneNode>( node->name ? node->name : "UnnamedNode" );

        // Set transform
        if ( node->has_matrix )
        {
            // Decompose matrix into translation, rotation, scale
            glm::mat4 matrix = glm::make_mat4( node->matrix );
            glm::vec3 scale;
            glm::quat rotation;
            glm::vec3 translation;
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::decompose( matrix, scale, rotation, translation, skew, perspective );

            newNode->m_transform.m_translation = translation;
            newNode->m_transform.m_rotation    = rotation;
            newNode->m_transform.m_scale       = scale;
        }
        else
        {
            if ( node->has_translation )
            {
                newNode->m_transform.m_translation = glm::vec3(
                        node->translation[0],
                        node->translation[1],
                        node->translation[2]
                        );
            }

            if ( node->has_rotation )
            {
                newNode->m_transform.m_rotation = glm::quat(
                        node->rotation[3], // w
                        node->rotation[0], // x
                        node->rotation[1], // y
                        node->rotation[2] // z
                        );
            }

            if ( node->has_scale )
            {
                newNode->m_transform.m_scale = glm::vec3(
                        node->scale[0],
                        node->scale[1],
                        node->scale[2]
                        );
            }
        }
        newNode->m_transform.update();

        // Process mesh if this node has one
        if ( node->mesh )
        {
            processMesh( node->mesh, data, newNode, shader, materials );
        }

        // Process children recursively
        for ( size_t i = 0; i < node->children_count; i++ )
        {
            processNode( node->children[i], data, newNode, shader, materials );
        }

        parentNode->addChild( newNode );
    }

    void GLBLoader::processMesh( cgltf_mesh*                             mesh, cgltf_data* data,
                                 const std::shared_ptr<Node>&            node,
                                 const std::shared_ptr<Shader>&          shader,
                                 std::vector<std::shared_ptr<Material>>& materials )
    {
        for ( size_t i = 0; i < mesh->primitives_count; i++ )
        {
            cgltf_primitive* primitive = &mesh->primitives[i];

            std::vector<float>    positions;
            std::vector<float>    normals;
            std::vector<float>    texCoords;
            std::vector<float>    tangents;
            std::vector<uint32_t> indices;

            // Process attributes
            for ( size_t j = 0; j < primitive->attributes_count; j++ )
            {
                cgltf_attribute*   attribute = &primitive->attributes[j];
                cgltf_accessor*    accessor  = attribute->data;
                cgltf_buffer_view* view      = accessor->buffer_view;
                uint8_t*           data_ptr  = ( uint8_t* ) view->buffer->data;

                if ( attribute->type == cgltf_attribute_type_position )
                {
                    positions.resize( accessor->count * 3 );
                    for ( size_t k = 0; k < accessor->count; k++ )
                    {
                        cgltf_accessor_read_float( accessor, k, &positions[k * 3], 3 );
                    }
                }
                else if ( attribute->type == cgltf_attribute_type_normal )
                {
                    normals.resize( accessor->count * 3 );
                    for ( size_t k = 0; k < accessor->count; k++ )
                    {
                        cgltf_accessor_read_float( accessor, k, &normals[k * 3], 3 );
                    }
                }
                else if (attribute->type == cgltf_attribute_type_texcoord)
                {
                    texCoords.resize(accessor->count * 2);
                    for (size_t k = 0; k < accessor->count; k++)
                    {
                        cgltf_accessor_read_float(accessor, k, &texCoords[k * 2], 2);
                        // Flip Y coordinate for OpenGL
                        texCoords[k * 2 + 1] = 1.0f - texCoords[k * 2 + 1];
                    }
                }
                else if ( attribute->type == cgltf_attribute_type_tangent )
                {
                    tangents.resize( accessor->count * 4 );
                    for ( size_t k = 0; k < accessor->count; k++ )
                    {
                        cgltf_accessor_read_float( accessor, k, &tangents[k * 4], 4 );
                    }
                }
            }

            // Process indices
            if ( primitive->indices )
            {
                cgltf_accessor* accessor = primitive->indices;
                indices.resize( accessor->count );

                if ( accessor->component_type == cgltf_component_type_r_16u )
                {
                    const uint16_t* src = ( const uint16_t* ) ( ( uint8_t* ) accessor->buffer_view->buffer->data + accessor->offset + accessor->buffer_view->
                                                                offset );
                    for ( size_t k = 0; k < accessor->count; k++ )
                    {
                        indices[k] = src[k];
                    }
                }
                else if ( accessor->component_type == cgltf_component_type_r_32u )
                {
                    const uint32_t* src = ( const uint32_t* ) ( ( uint8_t* ) accessor->buffer_view->buffer->data + accessor->offset + accessor->buffer_view->
                                                                offset );
                    for ( size_t k = 0; k < accessor->count; k++ )
                    {
                        indices[k] = src[k];
                    }
                }
                else
                {
                    LOG_WARN() << "Unsupported index component type in GLB file";
                    indices.resize( 0 );
                }
            }

            // Create buffers
            auto positionBuffer = std::make_shared<VertexBuffer>(
                    positions.data(),
                    positions.size() * sizeof( float ) );

            auto normalBuffer = normals.empty()
                                    ? nullptr
                                    : std::make_shared<VertexBuffer>(
                                            normals.data(),
                                            normals.size() * sizeof( float ) );

            auto texCoordBuffer = texCoords.empty()
                                      ? nullptr
                                      : std::make_shared<VertexBuffer>(
                                              texCoords.data(),
                                              texCoords.size() * sizeof( float ) );

            auto tangentBuffer = tangents.empty()
                                     ? nullptr
                                     : std::make_shared<VertexBuffer>(
                                             tangents.data(),
                                             tangents.size() * sizeof( float ) );

            auto indexBuffer = indices.empty()
                                   ? nullptr
                                   : std::make_shared<IndexBuffer>(
                                           indices.data(),
                                           indices.size() );

            // Create mesh
            auto meshComponent = std::make_shared<Mesh>(
                    positionBuffer,
                    normalBuffer,
                    texCoordBuffer,
                    tangentBuffer,
                    indexBuffer,
                    shader );

            // Load material
            std::shared_ptr<Material> material;
            if ( primitive->material )
            {
                material = loadMaterial( primitive->material, shader, data );
                materials.push_back( material );
            }
            else
            {
                material = createDefaultMaterial( shader );
                materials.push_back( material );
            }

            // Create a MeshComponent and add it to the node
            auto meshComp = std::make_shared<MeshComponent>( meshComponent, material );
            node->addComponent( meshComp );
        }
    }

    std::shared_ptr<Material> GLBLoader::loadMaterial( cgltf_material*                cgltfMat,
                                                       const std::shared_ptr<Shader>& shader,
                                                       cgltf_data*                    data )
    {
        // Base color texture
        std::shared_ptr<Texture> baseTexture = nullptr;
        if ( cgltfMat->has_pbr_metallic_roughness &&
             cgltfMat->pbr_metallic_roughness.base_color_texture.texture )
        {
            baseTexture = loadTexture( cgltfMat->pbr_metallic_roughness.base_color_texture.texture, data );
        }

        // Normal map
        std::shared_ptr<Texture> normalTexture = nullptr;
        if ( cgltfMat->normal_texture.texture )
        {
            normalTexture = loadTexture( cgltfMat->normal_texture.texture, data );
        }

        // Metallic/roughness texture
        std::shared_ptr<Texture> metallicRoughnessTexture = nullptr;
        if ( cgltfMat->has_pbr_metallic_roughness &&
             cgltfMat->pbr_metallic_roughness.metallic_roughness_texture.texture )
        {
            metallicRoughnessTexture = loadTexture( cgltfMat->pbr_metallic_roughness.metallic_roughness_texture.texture, data );
        }

        // Emission texture
        std::shared_ptr<Texture> emissionTexture = nullptr;
        if ( cgltfMat->emissive_texture.texture )
        {
            emissionTexture = loadTexture( cgltfMat->emissive_texture.texture, data );
        }

        // Material properties
        glm::vec4 baseColor( 1.0f );
        if ( cgltfMat->has_pbr_metallic_roughness )
        {
            baseColor = glm::vec4(
                    cgltfMat->pbr_metallic_roughness.base_color_factor[0],
                    cgltfMat->pbr_metallic_roughness.base_color_factor[1],
                    cgltfMat->pbr_metallic_roughness.base_color_factor[2],
                    cgltfMat->pbr_metallic_roughness.base_color_factor[3]
                    );
        }

        float metallic = cgltfMat->has_pbr_metallic_roughness ? cgltfMat->pbr_metallic_roughness.metallic_factor : 0.0f;

        float roughness = cgltfMat->has_pbr_metallic_roughness ? cgltfMat->pbr_metallic_roughness.roughness_factor : 1.0f;

        glm::vec3 emission( 0.0f );
        if ( cgltfMat->has_emissive_strength )
        {
            emission = glm::vec3(
                    cgltfMat->emissive_factor[0] * cgltfMat->emissive_strength.emissive_strength,
                    cgltfMat->emissive_factor[1] * cgltfMat->emissive_strength.emissive_strength,
                    cgltfMat->emissive_factor[2] * cgltfMat->emissive_strength.emissive_strength
                    );
        }
        else if ( cgltfMat->emissive_factor )
        {
            emission = glm::vec3(
                    cgltfMat->emissive_factor[0],
                    cgltfMat->emissive_factor[1],
                    cgltfMat->emissive_factor[2]
                    );
        }

        // Alpha mode
        AlphaMode alphaMode = AlphaMode::OPAQUE;
        if ( cgltfMat->alpha_mode == cgltf_alpha_mode_mask )
        {
            alphaMode = AlphaMode::MASK;
        }
        else if ( cgltfMat->alpha_mode == cgltf_alpha_mode_blend )
        {
            alphaMode = AlphaMode::BLEND;
        }

        float alphaCutoff = cgltfMat->alpha_cutoff;

        return std::make_shared<Material>(
                shader,
                baseColor,
                emission,
                metallic,
                roughness,
                1.0f, // normal scale
                alphaCutoff,
                alphaMode,
                baseTexture ? baseTexture : Material::getDefaultWhite(),
                metallicRoughnessTexture,
                emissionTexture,
                normalTexture,
                Sampler::createLinearRepeat(),
                cgltfMat->name
                    ? std::string( "GLB_Material_" ) + cgltfMat->name
                    : std::string( "GLB_Material_" ) + std::to_string( ResourceManager::getNewResourceId() )
                );
    }

    std::shared_ptr<Texture> GLBLoader::loadTexture( cgltf_texture* texture, cgltf_data* data )
    {
        if ( !texture || !texture->image )
        {
            return nullptr;
        }

        cgltf_image* image = texture->image;

        // Handle embedded texture
        if ( image->buffer_view )
        {
            const uint8_t* data_ptr = ( const uint8_t* ) image->buffer_view->buffer->data +
                                      image->buffer_view->offset;
            size_t data_size = image->buffer_view->size;

            return Texture::createFromMemory(
                    data_ptr,
                    data_size,
                    TextureFormat::SRGBA8
                    );
        }
        else if ( image->uri )
        {
            // Handle external texture (shouldn't happen with GLB)
            std::string fullPath = ResourceManager::getTexturePath( image->uri );
            return Texture::createFromFile( fullPath, TextureFormat::SRGBA8 );
        }

        return nullptr;
    }

    std::shared_ptr<Material> GLBLoader::createDefaultMaterial( const std::shared_ptr<Shader>& shader )
    {
        return std::make_shared<Material>(
                shader,
                glm::vec4( 1.0f ), // white
                glm::vec3( 0.0f ), // no emission
                0.0f, // non-metallic
                1.0f, // fully rough
                1.0f, // normal scale
                0.5f, // alpha cutoff
                AlphaMode::OPAQUE,
                Material::getDefaultWhite(),
                Material::getDefaultWhite(),
                nullptr,
                Material::getDefaultNormal(),
                Sampler::createLinearRepeat(),
                "DefaultMaterial"
                );
    }
} // namespace Tomos
