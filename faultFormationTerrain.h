#pragma once
#include "baseTerrain.h"

class FaultFormationTerrain : public BaseTerrain
{
public:
	FaultFormationTerrain(){}

	void createFaultFormation(int terrainSize, int iterations, float minHeight, float maxHeight, float filter);
	
private:
	struct TerrainPoint
	{
		int x = 0;
		int z = 0;

		bool isEqual(TerrainPoint& p) const
		{
			return ((x == p.x) && (z == p.z));
		}
	};

	void createFualtFormationInternal(int iterations, float minHeight, float maxHeight, float filter);
	void getRandomTerrainPoints(TerrainPoint& p1, TerrainPoint& p2);
	void applyFIRFilter(float filter);
	float FIRFilterSinglePoint(int x, int z, float prevVal, float filter);
};

