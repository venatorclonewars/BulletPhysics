#include "faultFormationTerrain.h"

void FaultFormationTerrain::createFaultFormation(int terrainSize, int iterations, float minHeight, float maxHeight, float filter)
{
	m_terrainSize = terrainSize;
	m_minHeight = minHeight;
	m_maxHeight = maxHeight;

	m_terrainTech->enable();
	m_terrainTech->setMinMaxHeight(minHeight, maxHeight);
	m_terrainTech->setTerrainTexture(0);

	m_heightMap.setArray2D(terrainSize, terrainSize, 0.0f);

	createFualtFormationInternal(iterations, minHeight, maxHeight, filter);

	m_heightMap.normalize(minHeight, maxHeight);

	//m_geomipGrid.createGeomipGrid(m_terrainSize, m_terrainSize, 2, this);
}


void FaultFormationTerrain::createFualtFormationInternal(int iterations, float minHeight, float maxHeight, float filter)
{
	float deltaHeight = maxHeight - minHeight;

	for (int curIter = 0; curIter < iterations; curIter++)
	{
		float iterRatio = ((float)curIter / (float)iterations);
		float height = maxHeight - iterRatio * deltaHeight;

		TerrainPoint p1, p2;

		getRandomTerrainPoints(p1, p2);

		int dirX = p2.x - p1.x;
		int dirZ = p2.z - p1.z;

		for (int z = 0; z < m_terrainSize; z++)
		{
			for (int x = 0; x < m_terrainSize; x++)
			{
				int dirX_in = x - p1.x;
				int dirZ_in = z - p1.z;

				int crossProd = dirX_in * dirZ - dirX * dirZ_in;

				if (crossProd > 0)
				{
					float curHeight = m_heightMap.get(x, z);
					m_heightMap.set(x, z, curHeight + height);
				}
			}
		}

	}


	applyFIRFilter(filter);
}

void FaultFormationTerrain::getRandomTerrainPoints(TerrainPoint& p1, TerrainPoint& p2)
{
	p1.x = rand() % m_terrainSize;
	p1.z = rand() % m_terrainSize;

	int counter = 0;

	do 
	{
		p2.x = rand() % m_terrainSize;
		p2.z = rand() % m_terrainSize;

		if (counter++ == 1000)
			assert(0);
	} while (p1.isEqual(p2));
}

void FaultFormationTerrain::applyFIRFilter(float filter)
{
	for (int z = 0; z < m_terrainSize; z++)
	{
		float prevVal = m_heightMap.get(0, z);
		for (int x = 1; x < m_terrainSize; x++)
		{
			prevVal = FIRFilterSinglePoint(x, z, prevVal, filter);
		}
	}

	for (int z = 0; z < m_terrainSize; z++)
	{
		float prevVal = m_heightMap.get(m_terrainSize - 1, z);
		for (int x = m_terrainSize - 2; x >= 0; x--)
		{
			prevVal = FIRFilterSinglePoint(x, z, prevVal, filter);
		}
	}

	for (int x = 0; x < m_terrainSize; x++)
	{
		float prevVal = m_heightMap.get(x, 0);
		for (int z = 1; z < m_terrainSize; z++)
		{
			prevVal = FIRFilterSinglePoint(x, z, prevVal, filter);
		}
	}

	for (int x = 0; x < m_terrainSize; x++)
	{
		float prevVal = m_heightMap.get(x, m_terrainSize - 1);
		for (int z = m_terrainSize - 2; z >= 0; z--)
		{
			prevVal = FIRFilterSinglePoint(x, z, prevVal, filter);
		}
	}
}

float FaultFormationTerrain::FIRFilterSinglePoint(int x, int z, float prevVal, float filter)
{
	float curVal = m_heightMap.get(x, z);
	float newVal = filter * prevVal + (1 - filter) * curVal;
	m_heightMap.set(x, z, newVal);
	return newVal;
}
