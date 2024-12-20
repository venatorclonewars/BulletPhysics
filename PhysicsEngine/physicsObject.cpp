#include "physicsObject.h"

//PhysicsObject::PhysicsObject(const Vector3f& position, const Vector3f& velocity, Cube* cube)
//{
//	m_position = position;
//	m_velocity = velocity;
//	m_cube = cube;
//	m_radius = 0.5f;
//	m_boundingSphere(position, m_radius);
//}

void PhysicsObject::integrate(float delta)
{
	m_position += m_velocity * delta;
	m_cube->setPosition(m_position);
}
