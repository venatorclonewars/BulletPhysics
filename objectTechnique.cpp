#include "objectTechnique.h"

ObjectTechnique::ObjectTechnique()
{
}

bool ObjectTechnique::init()
{
	if (!Technique::init())
		return false;

	if (!addShader(GL_VERTEX_SHADER, "objectShader.vs"))
		return false;

	if (!addShader(GL_FRAGMENT_SHADER, "objectShader.fs"))
		return false;

	if (!finalize())
		return false;

	WVPLoc = getUniformLocation("gWVP");
	lightDirLoc = getUniformLocation("gLightDir");


	enable();
}

void ObjectTechnique::setWVP(const Matrix4f& WVP)
{
	glUniformMatrix4fv(WVPLoc, 1, GL_TRUE, (const GLfloat*)WVP.mat);
}

void ObjectTechnique::setLightDir(const Vector3f& lightDir)
{
	
	glUniform3f(lightDirLoc, lightDir.x, lightDir.y, lightDir.z);
	
}
