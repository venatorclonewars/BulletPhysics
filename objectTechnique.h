#pragma once
#include "math.h"
#include "../technique.h"

class ObjectTechnique : public Technique
{
public:
	ObjectTechnique();

	virtual bool init();

	void setWVP(const Matrix4f& WVP);

	void setModelMatrix(const Matrix4f& modelMatrix);

	void setLightDir(const Vector3f& lightDir);

private:
	GLuint WVPLoc;
	GLuint modelMatLoc;
	GLuint lightDirLoc;

};

