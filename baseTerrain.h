#pragma once
#include "math.h"
#include "terrainTehnique.h"
#include "lightingTechnique.h"
#include "singleTexTerrainTechnique.h"
#include "camera.h"
#include "geomipGrid.h"
#include "texture.h"

class BaseTerrain
{
public:
	void initTerrain();
	void initTerrain(float worldScale, float textureScale, const std::vector<string>& textureFilenames);
	void initTerrain(float worldScale, float textureScale);

	void render(const Matrix4f& WVP, const DirectionalLight& dirLight, const Vector3f& cameraPos);
	void render(Matrix4f& projection, Matrix4f& view, Matrix4f _view, const DirectionalLight& dirLight, const Vector3f& cameraPos);

	void loadFromFile(const char* fileName);

	float getHeight(int x, int z) const { return m_heightMap.get(x, z); }

	int getSize() const { return m_terrainSize; }

	float getHeightInterpolated(float x, float z) const;

	float getWorldScale() const { return m_worldScale; }

	float getTextureScale() const { return m_textureScale; }

	void setTexture(Texture* pTexture) { m_pTextures[0] = pTexture; }

	void setMinMaxHeight(float minHeight, float maxHeight);

	void setLightDirection(const DirectionalLight& dirLight);

	void setTextureHeights(float tex0Height, float tex1Height, float tex2Height, float tex3Height);

	Vector3f constrainCameraPosToTerrain(const Vector3f& camPos);
	float getWorldHeight(float x, float z) const;

	float getWorldSize() const { return m_terrainSize * m_worldScale; }

protected:
	void loadHeightMapFromFile(const char* fileName);

	int m_terrainSize;
	Array2D<float> m_heightMap;
	TerrainTehnique* m_terrainTech = NULL;
	SingleTexTerrainTechnique* m_singleTerrainTech = NULL;
	

	GeomipGrid m_geomipGrid;

	float m_minHeight = 0.0f;
	float m_maxHeight = 0.0f;
	float m_worldScale = 1.0f;
	float m_textureScale = 1.0f;
	bool m_isSingleTexTerrain = false;

	Texture* m_pTextures[4] = { 0 };

	float m_cameraHeight = 3.0f;
};


