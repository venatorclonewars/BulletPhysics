#include "textureGenerator.h"
#include "baseTerrain.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define SNPRINTF snprintf
#include "stb_image_write.h"

TextureGenerator::TextureGenerator()
{
}

void TextureGenerator::loadTile(const char* filename)
{
	m_textureTiles[m_numTextureTiles].image.load(filename);
	m_numTextureTiles++;
}

Texture* TextureGenerator::generateTexture(int textureSize, BaseTerrain* pTerrain, float minHeight, float maxHeight)
{
	if (m_numTextureTiles == 0)
		exit(0);

	calcTextureRegions(minHeight, maxHeight);

	int BPP = 3;
	int textureBytes = textureSize * textureSize * BPP;
	unsigned char* pTextureData = (unsigned char*)malloc(textureBytes);
	unsigned char* p = pTextureData;

	float heightMapToTextureRatio = (float)pTerrain->getSize() / (float)textureSize;

	for (int y = 0; y < textureSize; y++)
	{
		for (int x = 0; x < textureSize; x++)
		{
			float interpolatedHeight = pTerrain->getHeightInterpolated((float)x * heightMapToTextureRatio, (float)y * heightMapToTextureRatio);
			//printf("Height at (%d, %d): %f\n", x, y, interpolatedHeight);

			float red = 0.0f;
			float green = 0.0f;
			float blue = 0.0f;

			for (int tile = 0; tile < m_numTextureTiles; tile++)
			{
				Vector3f color = m_textureTiles[tile].image.getColor(x, y);

				float blendFactor = regionPercent(tile, interpolatedHeight);

				red += blendFactor * color.x;
				green += blendFactor * color.y;
				blue += blendFactor * color.z;

				
			}
			p[0] = (unsigned char)red;
			p[1] = (unsigned char)green;
			p[2] = (unsigned char)blue;
			//printf("Red: %d, Green: %d, Blue: %d\n", (unsigned char)red, (unsigned char)green, (unsigned char)blue);

			p += 3;
		}
	
	}

	Texture* pTexture = new Texture(GL_TEXTURE_2D);

	stbi_write_png("texture.png", textureSize, textureSize, BPP, pTextureData, textureSize * BPP);
	pTexture->loadRaw(textureSize, textureSize, BPP, pTextureData);

	free(pTextureData);

	return pTexture;

}

void TextureGenerator::calcTextureRegions(float minHeight, float maxHeight)
{
	float heightRange = maxHeight - minHeight;

	float rangePerTile = heightRange / m_numTextureTiles;
	float remainder = heightRange - rangePerTile * m_numTextureTiles;

	if (remainder < 0.0f)
	{
		printf("%s:%d: negative remainder %f (num tiles %d range per tile %f)\n", __FILE__, __LINE__, remainder, m_numTextureTiles, rangePerTile);
		exit(0);
	}

	float lastHeight = -1.0f;

	for (int i = 0; i < m_numTextureTiles; i++) 
	{
		m_textureTiles[i].heightDesc.low = lastHeight + 1;
		lastHeight += rangePerTile;
		m_textureTiles[i].heightDesc.optimal = lastHeight;
		m_textureTiles[i].heightDesc.high = m_textureTiles[i].heightDesc.optimal + rangePerTile;

		m_textureTiles[i].heightDesc.Print(); printf("\n");
	}
}

float TextureGenerator::regionPercent(int tile, float height)
{
	float percent = 0.0f;

	if (height < m_textureTiles[tile].heightDesc.low) {
		percent = 0.0f;
	}
	else if (height > m_textureTiles[tile].heightDesc.high) 
	{
		percent = 0.0f;
	}
	else if (height < m_textureTiles[tile].heightDesc.optimal) 
	{
		float nom = (float)height - (float)m_textureTiles[tile].heightDesc.low;
		float denom = (float)m_textureTiles[tile].heightDesc.optimal - (float)m_textureTiles[tile].heightDesc.low;
		percent = nom / denom;
	}
	else if (height >= m_textureTiles[tile].heightDesc.optimal) 
	{
		float nom = (float)m_textureTiles[tile].heightDesc.high - (float)height;
		float denom = (float)m_textureTiles[tile].heightDesc.high - (float)m_textureTiles[tile].heightDesc.optimal;
		percent = nom / denom;
	}
	else
	{
		printf("%s:%d - shouldn't get here! tile %d Height %f\n", __FILE__, __LINE__, tile, height);
		exit(0);
	}

	if ((percent < 0.0f) || (percent > 1.0f)) 
	{
		printf("%s:%d - Invalid percent %f\n", __FILE__, __LINE__, percent);
		exit(0);
	}

	return percent;
}
