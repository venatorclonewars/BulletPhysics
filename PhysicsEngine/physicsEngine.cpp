#include "physicsEngine.h"
#include "intersectData.h"

void PhysicsEngine::addObject(const PhysicsObject& object)
{
	m_objects.push_back(object);
}

void PhysicsEngine::simulate(float delta)
{
	delta /= 1000000;
	for (int i = 0; i < m_objects.size(); i++)
		m_objects[i].integrate(delta);
}

void PhysicsEngine::handleCollisions()
{
	for (int i = 0; i < m_objects.size(); i++)
	{
		for (int j = i + 1; j < m_objects.size(); j++)
		{
			IntersectData intersectData = 
				m_objects[i].getCollider().intersect(
					m_objects[j].getCollider());
			
			if (intersectData.getDoesIntersect())
			{
				m_objects[i].setVelocity(m_objects[i].getVelocity() * -1);
				m_objects[j].setVelocity(m_objects[i].getVelocity() * -1);
			}
		}
	}
		
}
