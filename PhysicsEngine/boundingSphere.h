#pragma once
#include "../math.h"
#include "intersectData.h"
#include "collider.h"

class BoundingSphere : public Collider
{
public:
	BoundingSphere(const Vector3f& center, float radius)
		: Collider(Collider::TYPE_SPHERE), m_center(center), m_radius(radius) {}

	IntersectData intersectBoundingSphere(const BoundingSphere& sphere);

	virtual void transform(const Vector3f& translation);

	virtual Vector3f getCenter() const { return m_center; }
	inline const float getRadius() const { return m_radius; }



private:
	Vector3f m_center;
	float m_radius;
};

