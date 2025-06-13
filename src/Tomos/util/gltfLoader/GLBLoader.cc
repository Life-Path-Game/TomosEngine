#define GLM_ENABLE_EXPERIMENTAL
#define CGLTF_IMPLEMENTATION

#include "GLBLoader.hh"

#include <filesystem>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <unordered_map>

#include "Tomos/systems/mesh/MeshComponent.hh"
#include "Tomos/util/logger/Logger.hh"
#include "Tomos/util/renderer/Buffer.hh"

namespace Tomos
{
    GLBLoader::LoadResult GLBLoader::loadGLB( const std::string& p_filepath, const std::shared_ptr<Shader>& p_shader, bool p_useCache )
    {
        LoadResult result;

        cgltf_options options     = {};
        cgltf_data*   data        = nullptr;
        cgltf_result  parseResult = cgltf_parse_file( &options, p_filepath.c_str(), &data );

        if ( parseResult != cgltf_result_success )
        {
            LOG_ERROR() << "Failed to load GLB file: " << p_filepath << " Error: " << parseResult;
            return result;
        }

        // Load buffers
        cgltf_result loadResult = cgltf_load_buffers( &options, data, p_filepath.c_str() );
        if ( loadResult != cgltf_result_success )
        {
            LOG_ERROR() << "Failed to load GLB buffers: " << p_filepath << " Error: " << loadResult;
            cgltf_free( data );
            return result;
        }

        // Create root node
        auto rootNode     = std::make_shared<Node>( "GLB_Root_" + p_filepath.substr( p_filepath.find_last_of( "/\\" ) + 1 ) );
        result.m_rootNode = rootNode;

        // Process all nodes recursively
        for ( size_t i = 0; i < data->nodes_count; i++ )
        {
            if ( !data->nodes[i].parent ) // Only process root nodes, children will be processed recursively
            {
                processNode( &data->nodes[i], data, rootNode, p_shader, result.m_materials, p_useCache );
            }
        }

        // Free cgltf data
        cgltf_free( data );

        return result;
    }

    std::shared_ptr<Node> GLBLoader::createInstance( const std::shared_ptr<Node>& p_original )
    {
        if ( !p_original ) return nullptr;

        // Create a deep copy of the node hierarchy
        return deepCopyNode( p_original );
    }

    std::shared_ptr<Node> GLBLoader::deepCopyNode( const std::shared_ptr<Node>& p_original )
    {
        auto copy = std::make_shared<Node>( p_original->m_name + "_Instance" );

        // Copy transform
        copy->m_transform = p_original->m_transform;

        for ( const auto& component : p_original->getComponents() )
        {
            if ( auto meshComp = std::dynamic_pointer_cast<MeshComponent>( component ) )
            {
                auto newMeshComp = std::make_shared<MeshComponent>( meshComp->getMesh(), meshComp->getMaterial() );
                copy->addComponent( newMeshComp );
            }
        }

        // Recursively copy children
        for ( const auto& child : p_original->getChildren() )
        {
            copy->addChild( deepCopyNode( child ) );
        }

        return copy;
    }

    void GLBLoader::processNode( cgltf_node* p_node, cgltf_data* p_data, const std::shared_ptr<Node>& p_parentNode, const std::shared_ptr<Shader>& p_shader,
                                 std::vector<std::shared_ptr<Material>>& p_materials, bool p_useCache )
    {
        // Create a new node
        auto newNode = std::make_shared<Node>( p_node->name ? p_node->name : "UnnamedNode" );

        // Set transform
        if ( p_node->has_matrix )
        {
            // Decompose matrix into translation, rotation, scale
            glm::mat4 matrix = glm::make_mat4( p_node->matrix );
            glm::vec3 scale;
            glm::quat rotation;
            glm::vec3 translation;
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::decompose( matrix, scale, rotation, translation, skew, perspective );

            //            newNode->m_transform.m_translation = translation;
            //            newNode->m_transform.m_rotation    = rotation;
            //            newNode->m_transform.m_scale       = scale;
        }
        else
        {
            if ( p_node->has_translation )
            {
                newNode->m_transform.m_translation = glm::vec3( p_node->translation[0], p_node->translation[1], p_node->translation[2] );
            }

            if ( p_node->has_rotation )
            {
                newNode->m_transform.m_rotation = glm::quat( p_node->rotation[3], // w
                                                             p_node->rotation[0], // x
                                                             p_node->rotation[1], // y
                                                             p_node->rotation[2] // z
                        );
            }

            if ( p_node->has_scale )
            {
                newNode->m_transform.m_scale = glm::vec3( p_node->scale[0], p_node->scale[1], p_node->scale[2] );
            }
        }
        newNode->m_transform.update();

        // Process mesh if this node has one
        if ( p_node->mesh )
        {
            processMesh( p_node->mesh, p_data, newNode, p_shader, p_materials, p_useCache );
        }

        // Process children recursively
        for ( size_t i = 0; i < p_node->children_count; i++ )
        {
            processNode( p_node->children[i], p_data, newNode, p_shader, p_materials, p_useCache );
        }

        p_parentNode->addChild( newNode );
    }

    void GLBLoader::processMesh( cgltf_mesh* p_mesh, cgltf_data* p_data, const std::shared_ptr<Node>& p_node, const std::shared_ptr<Shader>& p_shader,
                                 std::vector<std::shared_ptr<Material>>& p_materials, bool p_useCache )
    {
        for ( size_t i = 0; i < p_mesh->primitives_count; i++ )
        {
            cgltf_primitive* primitive = &p_mesh->primitives[i];

            // Generate a unique key for this mesh primitive
            std::string meshKey = std::string( p_mesh->name ? p_mesh->name : "unnamed" ) + "_primitive_" + std::to_string( i );

            std::shared_ptr<Mesh> meshComponent;

            // Check cache if enabled
            if ( p_useCache )
            {
                meshComponent = ResourceManager::getMesh( meshKey );
            }

            // If not in cache, create new mesh
            if ( !meshComponent )
            {
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
                    else if ( attribute->type == cgltf_attribute_type_texcoord )
                    {
                        texCoords.resize( accessor->count * 2 );
                        for ( size_t k = 0; k < accessor->count; k++ )
                        {
                            cgltf_accessor_read_float( accessor, k, &texCoords[k * 2], 2 );
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
                        const uint16_t* src =
                                ( const uint16_t* ) ( ( uint8_t* ) accessor->buffer_view->buffer->data + accessor->offset + accessor->buffer_view->offset );
                        for ( size_t k = 0; k < accessor->count; k++ )
                        {
                            indices[k] = src[k];
                        }
                    }
                    else if ( accessor->component_type == cgltf_component_type_r_32u )
                    {
                        const uint32_t* src =
                                ( const uint32_t* ) ( ( uint8_t* ) accessor->buffer_view->buffer->data + accessor->offset + accessor->buffer_view->offset );
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
                auto positionBuffer = std::make_shared<VertexBuffer>( positions.data(), positions.size() * sizeof( float ) );

                auto normalBuffer = normals.empty() ? nullptr : std::make_shared<VertexBuffer>( normals.data(), normals.size() * sizeof( float ) );

                auto texCoordBuffer = texCoords.empty() ? nullptr : std::make_shared<VertexBuffer>( texCoords.data(), texCoords.size() * sizeof( float ) );

                auto tangentBuffer = tangents.empty() ? nullptr : std::make_shared<VertexBuffer>( tangents.data(), tangents.size() * sizeof( float ) );

                auto indexBuffer = indices.empty() ? nullptr : std::make_shared<IndexBuffer>( indices.data(), indices.size() );

                // Create mesh
                meshComponent = std::make_shared<Mesh>( positionBuffer, normalBuffer, texCoordBuffer, tangentBuffer, indexBuffer, p_shader );

                // Add to cache if enabled
                if ( p_useCache )
                {
                    ResourceManager::cacheMesh( meshKey, meshComponent );
                }
            }

            // Load material (with caching)
            std::shared_ptr<Material> material;
            if ( primitive->material )
            {
                material = loadMaterial( primitive->material, p_shader, p_data, p_useCache );
            }
            else
            {
                material = createDefaultMaterial( p_shader );
            }

            p_materials.push_back( material );

            // Create a MeshComponent and add it to the node
            auto meshComp = std::make_shared<MeshComponent>( meshComponent, material );
            p_node->addComponent( meshComp );
        }
    }

    std::shared_ptr<Material> GLBLoader::loadMaterial( cgltf_material* p_cgltfMat, const std::shared_ptr<Shader>& p_shader, cgltf_data* p_data,
                                                       bool            p_useCache )
    {
        std::string materialKey = p_cgltfMat->name
                                      ? std::string( "GLB_Material_" ) + p_cgltfMat->name
                                      : std::string( "GLB_Material_" ) + std::to_string( ResourceManager::getNewResourceId() );

        // Check cache if enabled
        if ( p_useCache )
        {
            auto m = ResourceManager::getMaterial( materialKey );
            if ( m )
            {
                return m;
            }
        }

        // Load textures (with caching)
        std::shared_ptr<Texture> baseTexture = nullptr;
        if ( p_cgltfMat->has_pbr_metallic_roughness && p_cgltfMat->pbr_metallic_roughness.base_color_texture.texture )
        {
            baseTexture = loadTexture( p_cgltfMat->pbr_metallic_roughness.base_color_texture.texture, p_data, p_useCache );
        }

        std::shared_ptr<Texture> normalTexture = nullptr;
        if ( p_cgltfMat->normal_texture.texture )
        {
            normalTexture = loadTexture( p_cgltfMat->normal_texture.texture, p_data, p_useCache );
        }

        std::shared_ptr<Texture> metallicRoughnessTexture = nullptr;
        if ( p_cgltfMat->has_pbr_metallic_roughness && p_cgltfMat->pbr_metallic_roughness.metallic_roughness_texture.texture )
        {
            metallicRoughnessTexture = loadTexture( p_cgltfMat->pbr_metallic_roughness.metallic_roughness_texture.texture, p_data, p_useCache );
        }

        std::shared_ptr<Texture> emissionTexture = nullptr;
        if ( p_cgltfMat->emissive_texture.texture )
        {
            emissionTexture = loadTexture( p_cgltfMat->emissive_texture.texture, p_data, p_useCache );
        }

        // Material properties
        glm::vec4 baseColor( 1.0f );
        if ( p_cgltfMat->has_pbr_metallic_roughness )
        {
            baseColor = glm::vec4( p_cgltfMat->pbr_metallic_roughness.base_color_factor[0], p_cgltfMat->pbr_metallic_roughness.base_color_factor[1],
                                   p_cgltfMat->pbr_metallic_roughness.base_color_factor[2], p_cgltfMat->pbr_metallic_roughness.base_color_factor[3] );
        }

        float metallic = p_cgltfMat->has_pbr_metallic_roughness ? p_cgltfMat->pbr_metallic_roughness.metallic_factor : 0.0f;

        float roughness = p_cgltfMat->has_pbr_metallic_roughness ? p_cgltfMat->pbr_metallic_roughness.roughness_factor : 1.0f;

        glm::vec3 emission( 0.0f );
        if ( p_cgltfMat->has_emissive_strength )
        {
            emission = glm::vec3( p_cgltfMat->emissive_factor[0] * p_cgltfMat->emissive_strength.emissive_strength,
                                  p_cgltfMat->emissive_factor[1] * p_cgltfMat->emissive_strength.emissive_strength,
                                  p_cgltfMat->emissive_factor[2] * p_cgltfMat->emissive_strength.emissive_strength );
        }

        // Alpha mode
        AlphaMode alphaMode = AlphaMode::OPAQUE;
        if ( p_cgltfMat->alpha_mode == cgltf_alpha_mode_mask )
        {
            alphaMode = AlphaMode::MASK;
        }
        else if ( p_cgltfMat->alpha_mode == cgltf_alpha_mode_blend )
        {
            alphaMode = AlphaMode::BLEND;
        }

        float alphaCutoff = p_cgltfMat->alpha_cutoff;

        auto material = std::make_shared<Material>( p_shader, baseColor, emission, metallic, roughness,
                                                    1.0f, // normal scale
                                                    alphaCutoff, alphaMode, baseTexture ? baseTexture : Material::getDefaultWhite(), metallicRoughnessTexture,
                                                    emissionTexture, normalTexture, Sampler::createLinearRepeat(), materialKey );

        // Add to cache if enabled
        if ( p_useCache )
        {
            ResourceManager::cacheMaterial( materialKey, material );
        }

        return material;
    }

    std::shared_ptr<Texture> GLBLoader::loadTexture( cgltf_texture* p_texture, cgltf_data* p_data, bool p_useCache )
    {
        if ( !p_texture || !p_texture->image )
        {
            return nullptr;
        }

        cgltf_image* image = p_texture->image;

        // Generate a unique key for this texture
        std::string textureKey;
        if ( image->uri )
        {
            textureKey = std::string( image->uri );
        }
        else if ( image->buffer_view )
        {
            textureKey = "embedded_" + std::to_string( ( size_t ) image->buffer_view );
        }
        else
        {
            textureKey = "unknown_texture_" + std::to_string( ResourceManager::getNewResourceId() );
        }

        // Check cache if enabled
        if ( p_useCache )
        {
            auto cachedTexture = ResourceManager::getTexture( textureKey );
            if ( cachedTexture )
            {
                return cachedTexture;
            }
        }

        std::shared_ptr<Texture> textureObj = nullptr;

        // Handle embedded texture
        if ( image->buffer_view )
        {
            const uint8_t* data_ptr  = ( const uint8_t* ) image->buffer_view->buffer->data + image->buffer_view->offset;
            size_t         data_size = image->buffer_view->size;

            textureObj = Texture::createFromMemory( data_ptr, data_size, TextureFormat::SRGBA8 );
        }
        else if ( image->uri )
        {
            // Handle external texture
            std::string fullPath = ResourceManager::getTexturePath( image->uri );
            textureObj           = Texture::createFromFile( fullPath, TextureFormat::SRGBA8 );
        }

        // Add to cache if enabled and loaded successfully
        if ( p_useCache && textureObj )
        {
            ResourceManager::cacheTexture( textureKey, textureObj );
        }

        return textureObj;
    }

    std::shared_ptr<Material> GLBLoader::createDefaultMaterial( const std::shared_ptr<Shader>& p_shader )
    {
        return std::make_shared<Material>( p_shader,
                                           glm::vec4( 1.0f ), // white
                                           glm::vec3( 0.0f ), // no emission
                                           0.0f, // non-metallic
                                           1.0f, // fully rough
                                           1.0f, // normal scale
                                           0.5f, // alpha cutoff
                                           AlphaMode::OPAQUE, Material::getDefaultWhite(), Material::getDefaultWhite(), nullptr, Material::getDefaultNormal(),
                                           Sampler::createLinearRepeat(), "DefaultMaterial" );
    }
} // namespace Tomos
