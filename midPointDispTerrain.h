#pragma once
#include "baseTerrain.h"
#include "lightingTechnique.h"
#include "BulletPhysics/src/btBulletDynamicsCommon.h"

class MidpointDispTerrain : public BaseTerrain
{
public:
	void createMidpointDisplacement(int terrainSize, float roughness, float minHeight, float maxHeight, const DirectionalLight& dirLight, btDiscreteDynamicsWorld* dynamicsWorld);
	void createMidpointDisplacementF32(float roughness);
	void diamondStep(int rectSize, float curHeight);
	void squareStep(int rectSize, float curHeight);


};

