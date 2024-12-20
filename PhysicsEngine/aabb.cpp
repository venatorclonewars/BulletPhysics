#include "aabb.h"

IntersectData AABB::intersectAABB(const AABB& aabb) const
{
	Vector3f distance1 = aabb.getMinExtents() - m_maxExtents;
	Vector3f distance2 =  m_minExtents - aabb.getMaxExtents();
	Vector3f distance = distance1.maxVector(distance2);

	float maxDistance = distance.maxValue();

	return IntersectData(maxDistance < 0, maxDistance);
}
