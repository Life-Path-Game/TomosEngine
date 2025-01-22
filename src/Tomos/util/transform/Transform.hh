#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Tomos
{
    class Transform
    {
    public:
        Transform( const glm::vec3& translation = glm::vec3( 0.0f ),
                   const glm::quat& rotation    = glm::quat( 1.0f, 0.0f, 0.0f, 0.0f ),
                   const glm::vec3& scale       = glm::vec3( 1.0f ) );

        void update();
        void updateGlobal( const Transform& parent );

        const glm::mat4& getLocalMatrix();
        const glm::mat4& getLocalInvMatrix();

        glm::mat4 globMat;
        glm::mat4 globInvMat;

    private:
        void updateLocal();

        glm::mat4 localMat;
        glm::mat4 localInvMat;

        glm::vec3 translation;
        glm::quat rotation;
        glm::vec3 scale;

        bool dirty;
    };

}  // namespace Tomos
