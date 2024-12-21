#include "baseTerrain.h"
#include "util.h"

void BaseTerrain::initTerrain()
{
	m_terrainTech = new TerrainTehnique();
	m_terrainTech->init();
}

void BaseTerrain::initTerrain(float worldScale, float textureScale, const std::vector<string>& textureFilenames)
{
	m_terrainTech = new TerrainTehnique();
	m_terrainTech->init();

	m_worldScale = worldScale;
	m_textureScale = textureScale;
	m_isSingleTexTerrain = false;

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_pTextures); i++) 
	{
		m_pTextures[i] = new Texture(GL_TEXTURE_2D, textureFilenames[i]);
		m_pTextures[i]->load();
		
	}
}

void BaseTerrain::initTerrain(float worldScale, float textureScale)
{
	m_singleTerrainTech = new SingleTexTerrainTechnique();
	if (!m_singleTerrainTech->init())
	{
		printf("Error initializing tech\n");
		exit(0);
	}

	m_worldScale = worldScale;
	m_textureScale = textureScale;
	m_isSingleTexTerrain = true;
}

void BaseTerrain::render(const Matrix4f& WVP, const DirectionalLight& dirLight, const Vector3f& cameraPos)
{
	if (m_isSingleTexTerrain)
	{
		m_singleTerrainTech->enable();
		m_singleTerrainTech->setWVP(WVP);
		m_singleTerrainTech->setCamPosition(cameraPos);
		setLightDirection(dirLight);
	}
	else
	{
		m_terrainTech->enable();
		m_terrainTech->setWVP(WVP);
	}
	
	
	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_pTextures); i++)
	{
		if (m_pTextures[i])
			m_pTextures[i]->bindInternalDSA(COLOR_TEXTURE_UNIT_0 + i);
	}

	//m_geomipGrid.render();
	m_geomipGrid.render(cameraPos, WVP);

}

void BaseTerrain::render(Matrix4f& projection, Matrix4f& view, Matrix4f _view, const DirectionalLight& dirLight, const Vector3f& cameraPos)
{
	view.inverse();
	if (m_isSingleTexTerrain)
	{
		m_singleTerrainTech->enable();
		m_singleTerrainTech->setWVP(projection * view);
		setLightDirection(dirLight);
	}
	else
	{
		m_terrainTech->enable();
		m_terrainTech->setWVP(projection * view);
	}


	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_pTextures); i++)
	{
		if (m_pTextures[i])
			m_pTextures[i]->bindInternalDSA(COLOR_TEXTURE_UNIT_0 + i);
	}

	//m_geomipGrid.render();
	m_geomipGrid.render(cameraPos, projection, view, _view);
}

void BaseTerrain::loadFromFile(const char* fileName)
{
	loadHeightMapFromFile(fileName);

	//m_geomipGrid.createGeomipGrid(m_terrainSize, m_terrainSize, 3, this);
	//m_heightMap.print();
}

float BaseTerrain::getHeightInterpolated(float x, float z) const
{

	float baseHeight = getHeight((int)x, (int)z);

	if (((int)x + 1 >= m_terrainSize) || ((int)z + 1 >= m_terrainSize)) {
		return baseHeight;
	}

	float nextXHeight = getHeight((int)x + 1, (int)z);

	float ratioX = x - floorf(x);

	float interpolatedHeightX = (float)(nextXHeight - baseHeight) * ratioX + (float)baseHeight;

	float nextZHeight = getHeight((int)x, (int)z + 1);

	float ratioZ = z - floorf(z);

	float interpolatedHeightZ = (float)(nextZHeight - baseHeight) * ratioZ + (float)baseHeight;

	float finalHeight = (interpolatedHeightX + interpolatedHeightZ) / 2.0f;

	return finalHeight;
}

void BaseTerrain::setMinMaxHeight(float minHeight, float maxHeight)
{
	if (m_isSingleTexTerrain)
	{
		m_singleTerrainTech->setMinMaxHeight(minHeight, maxHeight);
	}
		
	else
	{
		m_terrainTech->setMinMaxHeight(minHeight, maxHeight);
	}
		
}

void BaseTerrain::setLightDirection(const DirectionalLight& dirLight)
{
	if (m_isSingleTexTerrain)
	{
		m_singleTerrainTech->setLightDirection(dirLight);
	}
}

void BaseTerrain::setTextureHeights(float tex0Height, float tex1Height, float tex2Height, float tex3Height)
{
	if (!m_isSingleTexTerrain)
	{
		//m_terrainTech->setTextureHeights();
	}

}

Vector3f BaseTerrain::constrainCameraPosToTerrain(const Vector3f& camPos)
{
	Vector3f newCamPos = camPos;

	// Make sure camera doesn't go outside of the terrain bounds
	if (camPos.x < 0.0f) 
	{
		newCamPos.x = 0.0f;
	}

	if (camPos.z < 0.0f) 
	{
		newCamPos.z = 0.0f;
	}

	if (camPos.x >= getWorldSize()) {
		newCamPos.x = getWorldSize() - 0.5f;
	}

	if (camPos.z >= getWorldSize()) {
		newCamPos.z = getWorldSize() - 0.5f;
	}

	newCamPos.y = getWorldHeight(camPos.x, camPos.z) + m_cameraHeight;


	return newCamPos;
}

float BaseTerrain::getWorldHeight(float x, float z) const
{
	float heightMapX = x / m_worldScale;
	float heightMapZ = z / m_worldScale;

	return getHeightInterpolated(heightMapX, heightMapZ);
}

void BaseTerrain::loadHeightMapFromFile(const char* fileName)
{
	int fileSize = 0;
	unsigned char* p = (unsigned char*)readBinaryFile(fileName, fileSize);

	m_terrainSize = sqrtf(fileSize / sizeof(float));
	m_heightMap.setArray2D(m_terrainSize, m_terrainSize, p);

}
