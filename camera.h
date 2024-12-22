#pragma once
#include "texture.h"
#include "lightingTechnique.h"
#include "skinnedMesh.h"
#include <algorithm>

class Game;

class Camera
{
public:
    Camera() {}
    ~Camera();

    Camera(Game* game);

    void setPosition(float x, float y, float z);
    void setPosition(Vector3f& pos);

    void onKeyboard(unsigned char key);
    void onMouse(int x, int y);

    void setWeightColorDebug(SkinnedMesh* pMesh, LightingTechnique* lightTech);

    Matrix4f getMatrix();

private:
    SkinnedMesh* m_pSkinnedMesh = NULL;
    LightingTechnique* m_pLightingTech = NULL;

    Vector3f m_forward;
    Vector3f m_up;
    Vector3f m_right;
    
    float m_speed = 1.0f; //2.8f;
    int m_displayBoneIndex = 0;

    Game* m_game;
public:
    float rotX = 0.0f;
    float rotY = 0.0f;
    Vector3f transformPos;
    Matrix4f cameraTransform;
};