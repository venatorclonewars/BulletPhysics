#pragma once
#include "../math.h"
#include "intersectData.h"

class AABB
{
public:
	AABB(const Vector3f& minExtents, const Vector3f& maxExtents)
		:m_minExtents(minExtents), m_maxExtents(maxExtents) {}

	IntersectData intersectAABB(const AABB& aabb) const;

	Vector3f getMinExtents() const { return m_minExtents; }
	Vector3f getMaxExtents() const { return m_maxExtents; }

private:
	Vector3f m_minExtents;
	Vector3f m_maxExtents;
};

