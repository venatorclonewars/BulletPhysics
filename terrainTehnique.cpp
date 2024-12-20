#include "terrainTehnique.h"
#include "prerequisites.h"

TerrainTehnique::TerrainTehnique()
{
}

bool TerrainTehnique::init()
{
	if (!Technique::init())
		return false;

	if (!addShader(GL_VERTEX_SHADER, "terrainShader.vs"))
		return false;

	if (!addShader(GL_FRAGMENT_SHADER, "terrainShader.fs"))
		return false;

	if (!finalize())
		return false;

	WVPLoc = getUniformLocation("gWVP");
	m_minHeightLoc = getUniformLocation("gMinHeight");
	m_maxHeightLoc = getUniformLocation("gMaxHeight");
	m_terrainTexLoc = getUniformLocation("gTerrainTexture");

	m_tex0UnitLoc = getUniformLocation("gTextureHeight0");
	m_tex1UnitLoc = getUniformLocation("gTextureHeight1");
	m_tex2UnitLoc = getUniformLocation("gTextureHeight2");
	m_tex3UnitLoc = getUniformLocation("gTextureHeight3");
	m_tex0HeightLoc = getUniformLocation("gHeight0");
	m_tex1HeightLoc = getUniformLocation("gHeight1");
	m_tex2HeightLoc = getUniformLocation("gHeight2");
	m_tex3HeightLoc = getUniformLocation("gHeight3");

	return true;
}

void TerrainTehnique::setWVP(const Matrix4f& WVP)
{
	glUniformMatrix4fv(WVPLoc, 1, GL_TRUE, (const GLfloat*)WVP.mat);
}

void TerrainTehnique::setMinMaxHeight(float min, float max)
{
	glUniform1f(m_minHeightLoc, min);
	glUniform1f(m_maxHeightLoc, max);
}

void TerrainTehnique::setTerrainTexture(GLuint textureUnit)
{
	glUniform1i(m_terrainTexLoc, textureUnit);
}

void TerrainTehnique::setTextureHeights(float tex0Height, float tex1Height, float tex2Height, float tex3Height)
{
	glUniform1f(m_tex0HeightLoc, tex0Height);
	glUniform1f(m_tex1HeightLoc, tex1Height);
	glUniform1f(m_tex2HeightLoc, tex2Height);
	glUniform1f(m_tex3HeightLoc, tex3Height);
}


