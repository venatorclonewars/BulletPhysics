#pragma once
#include "intersectData.h"
#include "../math.h"

class Collider
{
public:
	enum 
	{
		TYPE_SPHERE,
		TYPE_AABB,
		TYPE_SIZE
	};

	Collider(int type)
		: m_type(type) {}

	IntersectData intersect(const Collider& collider);
	virtual void transform(const Vector3f& translation) {}
	virtual Vector3f getCenter() const { return Vector3f(0.0f, 0.0f, 0.0f); }

	int getType() const { return m_type; }

private:
	int m_type;
};

