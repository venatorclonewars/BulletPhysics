#include "boundingSphere.h"

IntersectData BoundingSphere::intersectBoundingSphere(const BoundingSphere& sphere)
{
	float radiusDistance = m_radius + sphere.m_radius;
	float centerDistance = (sphere.getCenter() - m_center).length();

	return IntersectData(centerDistance < radiusDistance, centerDistance - radiusDistance);
}

void BoundingSphere::transform(const Vector3f& translation)
{
	m_center += translation;
}

