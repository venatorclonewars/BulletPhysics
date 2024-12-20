#pragma once
#include "technique.h"
#include "math.h"
#include "material.h"

#define MAX_BONES 100

class BaseLight
{
public:
	Vector3f color;
	float ambientIntensity;

	BaseLight()
	{
		color = Vector3f(1.0f, 1.0f, 1.0f);
		ambientIntensity = 0.0f;
	}

};

class DirectionalLight : public BaseLight
{
public:
	Vector3f worldDirection = Vector3f(0.0f, 0.0f, 0.0f);
	float diffuseIntensity = 0.0f;

	void calcLocalDirection(const Matrix4f world) 
	{
		Matrix3f world3f(world);

		Matrix3f  worldToLocal = world3f.transpose();

		localDirection = worldToLocal * worldDirection;

		localDirection = localDirection.normalize();
	}

	const Vector3f& getLocalDirection() const { return localDirection; }

private:
	Vector3f localDirection = Vector3f(0.0f, 0.0f, 0.0f);
};

class LightingTechnique : public Technique
{
public:
	LightingTechnique();

	virtual bool init();

	void setWVP(const Matrix4f& WVP);
	void setTextureUnit(unsigned int textureUnit);
	void setSpecularExpTextureUnit(unsigned int textureUnit);
	void setDirectionalLight(const DirectionalLight& light);
	void setCameraLocalPos(const Vector3f& cameraLocalPos);
	void setMaterial(const Material& material);
	void setDisplayBoneIndex(int boneIndex);
	void setBoneTransform(int index, const Matrix4f& transform);

private:
	GLuint WVPLoc;
	GLuint samplerLoc;
	GLuint samplerSpecularExpLoc;
	GLuint cameraLocalPosLoc;
	GLuint displayBoneIndex;
	GLuint m_boneLocation[MAX_BONES];

	struct 
	{
		GLuint ambientColor;
		GLuint diffuseColor;
		GLuint specularColor;
	} materialLoc;

	struct 
	{
		GLuint color;
		GLuint ambientIntensity;
		GLuint direction;
		GLuint diffuseIntensity;

	} dirLightLoc;
};

