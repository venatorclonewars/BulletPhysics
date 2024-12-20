#pragma once
#include "physicsObject.h"
#include <vector>

using namespace std;

class PhysicsEngine
{
public:
	PhysicsEngine() {}

	void addObject(const PhysicsObject& object);
	void simulate(float delta);
	void handleCollisions();


	PhysicsObject getObject(int index)
	{
		return m_objects[index];
	}

	int getNumObjects()
	{
		return m_objects.size();
	}

private:
	vector<PhysicsObject> m_objects;
};

