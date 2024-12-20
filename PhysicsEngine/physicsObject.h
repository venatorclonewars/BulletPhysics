#pragma once
#include "../math.h"
#include "../3DObjects/cube.h"
#include "boundingSphere.h"

class PhysicsObject
{
public:
	//PhysicsObject() {}
	//PhysicsObject(const Vector3f& position, const Vector3f& velocity, Cube* cube);
	PhysicsObject(Collider* collider, const Vector3f& velocity, Cube* cube)
		: m_position(collider->getCenter()), m_oldPosition(collider->getCenter()), m_velocity(velocity), m_cube(cube), m_collider(collider) {}
	void setVelocity(const Vector3f& velocity) { m_velocity = velocity; };
	void integrate(float delta);

	Vector3f getPosition() { return m_position; }
	Vector3f getVelocity() { return m_velocity; }

	Collider& getCollider()
	{
		Vector3f translation = m_position - m_oldPosition;
		m_oldPosition = m_position;
		m_collider->transform(translation);
		return *m_collider;
	}

private:
	Vector3f m_position;
	Vector3f m_oldPosition;
	Vector3f m_velocity;
	Cube* m_cube;

	Collider* m_collider;
};

