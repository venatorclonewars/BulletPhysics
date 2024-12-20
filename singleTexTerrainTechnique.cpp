#include "singleTexTerrainTechnique.h"
#include "prerequisites.h"

SingleTexTerrainTechnique::SingleTexTerrainTechnique()
{
}

bool SingleTexTerrainTechnique::init()
{
	if (!Technique::init())
		return false;

	if (!addShader(GL_VERTEX_SHADER, "terrainSingleTexShader.vs"))
		return false;

	if (!addShader(GL_FRAGMENT_SHADER, "terrainSingleTexShader.fs"))
		return false;

	if (!finalize())
		return false;

	WVPLoc = getUniformLocation("gWVP");
	m_minHeightLoc = getUniformLocation("gMinHeight");
	m_maxHeightLoc = getUniformLocation("gMaxHeight");
	m_terrainTexLoc = getUniformLocation("gTerrainTexture");
	m_lighDir = getUniformLocation("gLightDir");
	m_camPos = getUniformLocation("gCamPos");

	enable();

	glUniform1i(m_terrainTexLoc, 0);

	return true;
}

void SingleTexTerrainTechnique::setWVP(const Matrix4f& WVP)
{
	glUniformMatrix4fv(WVPLoc, 1, GL_TRUE, (const GLfloat*)WVP.mat);
}

void SingleTexTerrainTechnique::setMinMaxHeight(float min, float max)
{
	glUniform1f(m_minHeightLoc, min);
	glUniform1f(m_maxHeightLoc, max);
}

void SingleTexTerrainTechnique::setTerrainTexture(int textureUnit)
{
	glUniform1i(m_terrainTexLoc, textureUnit);
}

void SingleTexTerrainTechnique::setLightDirection(const DirectionalLight& dirLight)
{
	Vector3f localDirection = dirLight.getLocalDirection();

	glUniform3f(m_lighDir, localDirection.x, localDirection.y, localDirection.z);
}

void SingleTexTerrainTechnique::setCamPosition(const Vector3f& camPos)
{
	glUniform3f(m_camPos, camPos.x, camPos.y, camPos.z);
}


