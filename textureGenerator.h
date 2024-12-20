#pragma once
#include "texture.h"
#include "STBImage.h"


struct TextureHeightDesc
{
	float low = 0.0f;
	float optimal = 0.0f;
	float high = 0.0f;

	void Print() const { printf("Low %f Optimal %f High %f", low, optimal, high); }
};

struct TextureTile
{
	STBImage image;
	TextureHeightDesc heightDesc;
};

class BaseTerrain;

class TextureGenerator
{
public:
	TextureGenerator();

	void loadTile(const char* filename);

	Texture* generateTexture(int textureSize, BaseTerrain* pTerrain, float minHeight, float maxHeight);

	int getNumTextures() const { return m_numTextureTiles; }

private:

	void calcTextureRegions(float minHeight, float maxHeight);
	
	float regionPercent(int tile, float height);

#define MAX_TEXTURE_TILES 4

	TextureTile m_textureTiles[MAX_TEXTURE_TILES] = {};
	int m_numTextureTiles = 0;
};

