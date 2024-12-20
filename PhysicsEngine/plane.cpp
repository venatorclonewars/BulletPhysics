#include "plane.h"

Plane Plane::normalized() const
{
    float magnitude = m_normal.length();

    return Plane(m_normal.normalize(), m_distance / magnitude);
}

IntersectData Plane::intersectSphere(const BoundingSphere& sphere) const
{
    float distanceFromSphereCenter = fabs(m_normal.dot(sphere.getCenter()) + m_distance);
    float distanceFromSphere = distanceFromSphereCenter - sphere.getRadius();
    return IntersectData(distanceFromSphere < 0, distanceFromSphere);
}
