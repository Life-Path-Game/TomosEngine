//
// Created by dstuden on 4/9/25.
//

#include "GLBLoader.hh"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "Tomos/util/logger/Logger.hh"
#include "Tomos/util/renderer/Buffer.hh"

namespace Tomos
{
    GLBLoader::LoadResult GLBLoader::LoadGLB( const std::string&             filepath,
                                              const std::shared_ptr<Shader>& shader )
    {
        LoadResult       result;
        Assimp::Importer importer;

        unsigned int flags = aiProcess_Triangulate |
                             aiProcess_GenNormals |
                             aiProcess_CalcTangentSpace;

        const aiScene* scene = importer.ReadFile( filepath, flags );

        if ( !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode )
        {
            LOG_ERROR() << "Assimp error: " << importer.GetErrorString();
            return result;
        }

        if ( scene->mNumMeshes == 0 )
        {
            LOG_ERROR() << "No meshes found in GLB file: " << filepath;
            return result;
        }

        // Get first mesh
        aiMesh* aiMesh = scene->mMeshes[0];

        // Extract vertex data
        std::vector<float>    positions;
        std::vector<float>    normals;
        std::vector<float>    texCoords;
        std::vector<float>    tangents;
        std::vector<uint32_t> indices;

        // Positions
        positions.reserve( aiMesh->mNumVertices * 3 );
        for ( unsigned int i = 0; i < aiMesh->mNumVertices; i++ )
        {
            positions.push_back( aiMesh->mVertices[i].x );
            positions.push_back( aiMesh->mVertices[i].y );
            positions.push_back( aiMesh->mVertices[i].z );
        }

        // Normals
        normals.reserve( aiMesh->mNumVertices * 3 );
        if ( aiMesh->HasNormals() )
        {
            for ( unsigned int i = 0; i < aiMesh->mNumVertices; i++ )
            {
                normals.push_back( aiMesh->mNormals[i].x );
                normals.push_back( aiMesh->mNormals[i].y );
                normals.push_back( aiMesh->mNormals[i].z );
            }
        }

        // Texture Coordinates
        texCoords.reserve( aiMesh->mNumVertices * 2 );
        if ( aiMesh->HasTextureCoords( 0 ) )
        {
            for ( unsigned int i = 0; i < aiMesh->mNumVertices; i++ )
            {
                texCoords.push_back( aiMesh->mTextureCoords[0][i].x );
                texCoords.push_back( aiMesh->mTextureCoords[0][i].y );
            }
        }

        // Tangents
        tangents.reserve( aiMesh->mNumVertices * 3 );
        if ( aiMesh->HasTangentsAndBitangents() )
        {
            for ( unsigned int i = 0; i < aiMesh->mNumVertices; i++ )
            {
                tangents.push_back( aiMesh->mTangents[i].x );
                tangents.push_back( aiMesh->mTangents[i].y );
                tangents.push_back( aiMesh->mTangents[i].z );
            }
        }

        // Indices
        indices.reserve( aiMesh->mNumFaces * 3 );
        for ( unsigned int i = 0; i < aiMesh->mNumFaces; i++ )
        {
            aiFace face = aiMesh->mFaces[i];
            for ( unsigned int j = 0; j < face.mNumIndices; j++ )
            {
                indices.push_back( face.mIndices[j] );
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

        auto indexBuffer = std::make_shared<IndexBuffer>(
                indices.data(),
                indices.size() );

        // Create mesh
        result.mesh = std::make_shared<Mesh>(
                positionBuffer,
                normalBuffer,
                texCoordBuffer,
                tangentBuffer,
                indexBuffer,
                shader );

        // Load material
        if ( aiMesh->mMaterialIndex >= 0 )
        {
            aiMaterial* aiMat = scene->mMaterials[aiMesh->mMaterialIndex];
            result.material   = loadMaterial( aiMat, shader, scene );
        }
        else
        {
            result.material = createDefaultMaterial( shader );
        }

        return result;
    }

    std::shared_ptr<Material> GLBLoader::loadMaterial( aiMaterial*                    aiMat,
                                                       const std::shared_ptr<Shader>& shader,
                                                       const aiScene*                 scene )
    {
        // Base color texture
        std::shared_ptr<Texture> baseTexture = nullptr;
        aiString                 texPath;
        if ( aiMat->GetTexture( aiTextureType_DIFFUSE, 0, &texPath ) == AI_SUCCESS )
        {
            // Check for embedded texture
            if ( texPath.data[0] == '*' )
            {
                int texIndex = std::atoi( texPath.C_Str() + 1 );
                if ( texIndex >= 0 && texIndex < ( int ) scene->mNumTextures )
                {
                    baseTexture = loadEmbeddedTexture( scene->mTextures[texIndex] );
                }
            }
            else
            {
                // External texture (shouldn't happen with GLB)
                std::string fullPath = ResourceManager::getTexturePath( texPath.C_Str() );
                baseTexture          = Texture::createFromFile( fullPath, TextureFormat::RGBA16F );
            }
        }

        // Normal map
        std::shared_ptr<Texture> normalTexture = nullptr;
        if ( aiMat->GetTexture( aiTextureType_NORMALS, 0, &texPath ) == AI_SUCCESS ||
             aiMat->GetTexture( aiTextureType_HEIGHT, 0, &texPath ) == AI_SUCCESS )
        {
            if ( texPath.data[0] == '*' )
            {
                int texIndex = std::atoi( texPath.C_Str() + 1 );
                if ( texIndex >= 0 && texIndex < ( int ) scene->mNumTextures )
                {
                    normalTexture = loadEmbeddedTexture( scene->mTextures[texIndex] );
                }
            }
        }

        // Metallic/roughness
        std::shared_ptr<Texture> metallicRoughnessTexture = nullptr;
        if ( aiMat->GetTexture( aiTextureType_UNKNOWN, 0, &texPath ) == AI_SUCCESS )
        {
            if ( texPath.data[0] == '*' )
            {
                int texIndex = std::atoi( texPath.C_Str() + 1 );
                if ( texIndex >= 0 && texIndex < ( int ) scene->mNumTextures )
                {
                    metallicRoughnessTexture = loadEmbeddedTexture( scene->mTextures[texIndex] );
                }
            }
        }

        // Material properties
        aiColor3D baseColor( 1.0f, 1.0f, 1.0f );
        aiMat->Get( AI_MATKEY_COLOR_DIFFUSE, baseColor );

        float metallic = 0.0f;
        aiMat->Get( AI_MATKEY_METALLIC_FACTOR, metallic );

        float roughness = 1.0f;
        aiMat->Get( AI_MATKEY_ROUGHNESS_FACTOR, roughness );

        return std::make_shared<Material>(
                shader,
                glm::vec4( baseColor.r, baseColor.g, baseColor.b, 1.0f ),
                glm::vec3( 0.0f ), // emission
                metallic,
                roughness,
                1.0f, // normal scale
                0.5f, // alpha cutoff
                AlphaMode::OPAQUE,
                baseTexture ? baseTexture : Material::getDefaultWhite(),
                metallicRoughnessTexture,
                nullptr, // emission texture
                normalTexture,
                Sampler::createLinearRepeat(),
                std::string( "GLB_Material_" ) + std::to_string( ResourceManager::getNewResourceId() )
                );
    }

    std::shared_ptr<Texture> GLBLoader::loadEmbeddedTexture( const aiTexture* aiTex )
    {
        // Handle compressed embedded textures (most common in GLB)
        if ( aiTex->mHeight == 0 )
        {
            // Texture is compressed (e.g., PNG/JPG data)
            return Texture::createFromMemory(
                    reinterpret_cast<const unsigned char*>( aiTex->pcData ),
                    aiTex->mWidth, // This contains the size for compressed textures
                    TextureFormat::SRGBA8
                    );
        }
        else
        {
            // Uncompressed texture (rare in GLB)
            return Texture::createFromPixels(
                    reinterpret_cast<const unsigned char*>( aiTex->pcData ),
                    aiTex->mWidth,
                    aiTex->mHeight,
                    TextureFormat::SRGBA8
                    );
        }
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
