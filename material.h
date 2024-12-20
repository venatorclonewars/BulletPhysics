#pragma once
#include "math.h"
#include "texture.h"

class Material
{
public:
	Vector3f ambientColor = Vector3f(0.0f, 0.0f, 0.0f);
	Vector3f diffuseColor = Vector3f(0.0f, 0.0f, 0.0f);
	Vector3f specularColor = Vector3f(0.0f, 0.0f, 0.0f);

	Texture* pDiffuse = NULL;
	Texture* pSpecularExponent = NULL;
};