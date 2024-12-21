#include "midpointDispTerrain.h"
#include "prerequisites.h"

void MidpointDispTerrain::createMidpointDisplacement(int terrainSize, float roughness, float minHeight, float maxHeight, const DirectionalLight& dirLight, btDiscreteDynamicsWorld* dynamicsWorld)
{
    m_terrainSize = terrainSize;
    m_minHeight = minHeight;
    m_maxHeight = maxHeight;

    if(m_isSingleTexTerrain)
    {
        m_singleTerrainTech->enable();
        setMinMaxHeight(minHeight, maxHeight);
        setLightDirection(dirLight);
    }
    else
    {
        m_terrainTech->enable();
        setMinMaxHeight(minHeight, maxHeight);
        //m_terrainTech->setTerrainTexture(0);

    }
    
    m_heightMap.setArray2D(terrainSize, terrainSize, 0.0f);

    createMidpointDisplacementF32(roughness);

    m_heightMap.normalize(minHeight, maxHeight);

    m_geomipGrid.createGeomipGrid(m_terrainSize, m_terrainSize, 9, this, dynamicsWorld);
}

void MidpointDispTerrain::createMidpointDisplacementF32(float roughness)
{
    int rectSize = calcNextPowerOfTwo(m_terrainSize);
	float curHeight = (float)rectSize / 2.0f;
	float heightReduce = pow(2.0f, -roughness);

	while (rectSize > 0)
	{
		diamondStep(rectSize, curHeight);
		squareStep(rectSize, curHeight);

		rectSize /= 2;
		curHeight *= heightReduce;
	}
}

void MidpointDispTerrain::diamondStep(int rectSize, float curHeight)
{
    int halfRectSize = rectSize / 2;

    for (int y = 0; y < m_terrainSize; y += rectSize) 
    {
        for (int x = 0; x < m_terrainSize; x += rectSize) 
        {
            int next_x = (x + rectSize) % m_terrainSize;
            int next_y = (y + rectSize) % m_terrainSize;

            if (next_x < x) {
                next_x = m_terrainSize - 1;
            }

            if (next_y < y) {
                next_y = m_terrainSize - 1;
            }

            float TopLeft = m_heightMap.get(x, y);
            float TopRight = m_heightMap.get(next_x, y);
            float BottomLeft = m_heightMap.get(x, next_y);
            float BottomRight = m_heightMap.get(next_x, next_y);

            int mid_x = (x + halfRectSize) % m_terrainSize;
            int mid_y = (y + halfRectSize) % m_terrainSize;

            float RandValue = randomFloatRange(-curHeight, curHeight);
            float MidPoint = (TopLeft + TopRight + BottomLeft + BottomRight) / 4.0f;

            m_heightMap.set(mid_x, mid_y, MidPoint + RandValue);
        }
    }
}

void MidpointDispTerrain::squareStep(int rectSize, float curHeight)
{
    int halfRectSize = rectSize / 2;

    for (int y = 0; y < m_terrainSize; y += rectSize)
    {
        for (int x = 0; x < m_terrainSize; x += rectSize)
        {
            int next_x = (x + rectSize) % m_terrainSize;
            int next_y = (y + rectSize) % m_terrainSize;

            if (next_x < x) {
                next_x = m_terrainSize - 1;
            }

            if (next_y < y) {
                next_y = m_terrainSize - 1;
            }

            int mid_x = (x + halfRectSize) % m_terrainSize;
            int mid_y = (y + halfRectSize) % m_terrainSize;

            int prev_mid_x = (x - halfRectSize + m_terrainSize) % m_terrainSize;
            int prev_mid_y = (y - halfRectSize + m_terrainSize) % m_terrainSize;

            float CurTopLeft = m_heightMap.get(x, y);
            float CurTopRight = m_heightMap.get(next_x, y);
            float CurCenter = m_heightMap.get(mid_x, mid_y);
            float PrevYCenter = m_heightMap.get(mid_x, prev_mid_y);
            float CurBotLeft = m_heightMap.get(x, next_y);
            float PrevXCenter = m_heightMap.get(prev_mid_x, mid_y);

            float CurLeftMid = (CurTopLeft + CurCenter + CurBotLeft + PrevXCenter) / 4.0f + randomFloatRange(-curHeight, curHeight);
            float CurTopMid = (CurTopLeft + CurCenter + CurTopRight + PrevYCenter) / 4.0f + randomFloatRange(-curHeight, curHeight);

            m_heightMap.set(mid_x, y, CurTopMid);
            m_heightMap.set(x, mid_y, CurLeftMid);
        }
    }
}
