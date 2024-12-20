#pragma once
#include "../math.h"
#include "intersectData.h"
#include "boundingSphere.h"

class Plane
{
public:
	Plane(Vector3f normal, float distance)
		: m_normal(normal), m_distance(distance) {}

	Plane normalized() const;
	IntersectData intersectSphere(const BoundingSphere& sphere) const;


	Vector3f getNormal() { return m_normal; }
	float getDistance() { return m_distance; }

private:
	Vector3f m_normal;
	float m_distance;
};

