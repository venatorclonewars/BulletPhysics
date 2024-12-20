#pragma once
#include "technique.h"
#include "math.h"
#include <GL/glew.h>


class TerrainTehnique : public Technique
{
public:
	TerrainTehnique();

	virtual bool init();

	void setWVP(const Matrix4f& WVP);

	void setMinMaxHeight(float min, float max);	

	void setTerrainTexture(GLuint textureUnit);	
	
	void setTextureHeights(float tex0Height, float tex1Height, float tex2Height, float tex3Height);

private:
	GLuint WVPLoc = -1;
	GLuint m_minHeightLoc = -1;
	GLuint m_maxHeightLoc = -1;
	GLuint m_terrainTexLoc = -1;

	GLuint m_tex0HeightLoc = -1;
	GLuint m_tex1HeightLoc = -1;
	GLuint m_tex2HeightLoc = -1;
	GLuint m_tex3HeightLoc = -1;
	GLuint m_tex0UnitLoc = -1;
	GLuint m_tex1UnitLoc = -1;
	GLuint m_tex2UnitLoc = -1;
	GLuint m_tex3UnitLoc = -1;
};

