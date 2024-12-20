#include "lightingTechnique.h"
#include "material.h"
#include "prerequisites.h"

LightingTechnique::LightingTechnique()
{
}

bool LightingTechnique::init()
{
	if (!Technique::init())
		return false;

	if (!addShader(GL_VERTEX_SHADER, "shader.vs"))
		return false;

	if (!addShader(GL_FRAGMENT_SHADER, "shader.fs"))
		return false;

	if (!finalize())
		return false;

	WVPLoc = getUniformLocation("gWVP");
	samplerLoc = getUniformLocation("gSampler");
	samplerSpecularExpLoc = getUniformLocation("gSamplerSpecularExp");

	materialLoc.ambientColor = getUniformLocation("gMaterial.ambientColor");
	materialLoc.diffuseColor = getUniformLocation("gMaterial.diffuseColor");
	materialLoc.specularColor = getUniformLocation("gMaterial.specularColor");

	dirLightLoc.color = getUniformLocation("gDirectionalLight.color");
	dirLightLoc.ambientIntensity = getUniformLocation("gDirectionalLight.ambientIntensity");
	dirLightLoc.direction = getUniformLocation("gDirectionalLight.direction");
	dirLightLoc.diffuseIntensity = getUniformLocation("gDirectionalLight.diffuseIntensity");

	displayBoneIndex = getUniformLocation("gDisplayBoneIndex");

	cameraLocalPosLoc = getUniformLocation("gCameraLocalPos");

	for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_boneLocation); i++) 
	{
		char name[128];
		memset(name, 0, sizeof(name));
		snprintf(name, sizeof(name), "gBones[%d]", i);
		m_boneLocation[i] = getUniformLocation(name);
	}

	return true;
}

void LightingTechnique::setWVP(const Matrix4f& WVP)
{
	glUniformMatrix4fv(WVPLoc, 1, GL_TRUE, (const GLfloat*)WVP.mat);
}

void LightingTechnique::setTextureUnit(unsigned int textureUnit)
{
	glUniform1i(samplerLoc, textureUnit);
}

void LightingTechnique::setSpecularExpTextureUnit(unsigned int textureUnit)
{
	glUniform1i(samplerSpecularExpLoc, textureUnit);
}

void LightingTechnique::setDirectionalLight(const DirectionalLight& light)
{
	glUniform3f(dirLightLoc.color, light.color.x, light.color.y, light.color.z);
	glUniform1f(dirLightLoc.ambientIntensity, light.ambientIntensity);

	Vector3f localDirection = light.getLocalDirection();

	glUniform3f(dirLightLoc.direction, localDirection.x, localDirection.y, localDirection.z);
	glUniform1f(dirLightLoc.diffuseIntensity, light.diffuseIntensity);

}

void LightingTechnique::setCameraLocalPos(const Vector3f& cameraLocalPos)
{
	glUniform3f(cameraLocalPosLoc, cameraLocalPos.x, cameraLocalPos.y, cameraLocalPos.z);
}

void LightingTechnique::setMaterial(const Material& material)
{
	glUniform3f(materialLoc.ambientColor, material.ambientColor.x, material.ambientColor.y, material.ambientColor.z);
	glUniform3f(materialLoc.diffuseColor, material.diffuseColor.x, material.diffuseColor.y, material.diffuseColor.z);
	glUniform3f(materialLoc.specularColor, material.specularColor.x, material.specularColor.y, material.specularColor.z);
}

void LightingTechnique::setDisplayBoneIndex(int boneIndex)
{
	glUniform1i(displayBoneIndex, boneIndex);

}

void LightingTechnique::setBoneTransform(int index, const Matrix4f& transform)
{
	//assert(Index < MAX_BONES);
	if (index >= MAX_BONES) {
		return;
	}

	glUniformMatrix4fv(m_boneLocation[index], 1, GL_TRUE, &transform.mat[0][0]);
}
