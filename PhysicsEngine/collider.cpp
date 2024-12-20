#include "collider.h"
#include "boundingSphere.h"

IntersectData Collider::intersect(const Collider& collider)
{
    if (m_type == TYPE_SPHERE && collider.getType() == TYPE_SPHERE)
    {
        BoundingSphere* self = (BoundingSphere*)this;
        return self->intersectBoundingSphere((BoundingSphere&)collider);
    
    }

    return IntersectData(false, 0);
}
