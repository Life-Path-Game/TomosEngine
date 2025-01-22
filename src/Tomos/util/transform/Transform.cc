//
// Created by dstuden on 1/20/25.
//

#include "Transform.hh"

namespace Tomos
{
    Transform::Transform( const glm::vec3& translation, const glm::quat& rotation, const glm::vec3& scale ) :
        translation( translation ), rotation( rotation ), scale( scale ), dirty( false )
    {
        localMat    = glm::mat4( 1.0f );
        localInvMat = glm::mat4( 1.0f );
        globMat     = glm::mat4( 1.0f );
        globInvMat  = glm::mat4( 1.0f );
        updateLocal();
    }

    void Transform::update() { dirty = true; }

    void Transform::updateLocal()
    {
        localMat = glm::mat4( 1.0f );
        localMat = glm::translate( localMat, translation );
        localMat *= glm::mat4_cast( rotation );  // Convert quaternion to matrix and multiply
        localMat = glm::scale( localMat, scale );

        localInvMat = glm::inverse( localMat );
        dirty       = false;
    }

    void Transform::updateGlobal( const Transform& parent )
    {
        if ( dirty )
        {
            updateLocal();
        }
        globMat    = parent.globMat * localMat;
        globInvMat = glm::inverse( globMat );
    }

    const glm::mat4& Transform::getLocalMatrix()
    {
        if ( dirty )
        {
            updateLocal();
        }
        return localMat;
    }

    const glm::mat4& Transform::getLocalInvMatrix()
    {
        if ( dirty )
        {
            updateLocal();
        }
        return localInvMat;
    }
}  // namespace Tomos
