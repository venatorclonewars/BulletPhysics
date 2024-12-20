#pragma once
#include "math.h"
#include "../technique.h"

class ObjectTechnique : public Technique
{
public:
	ObjectTechnique();

	virtual bool init();

	void setWVP(const Matrix4f& WVP);


private:
	GLuint WVPLoc;

};

