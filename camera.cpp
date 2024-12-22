#include "camera.h"
#include <GL/freeglut.h>
#include "game.h"
#include <algorithm>

Camera::~Camera()
{
    delete m_pSkinnedMesh;
    delete m_pLightingTech;
}

Camera::Camera(Game* game)
{
    transformPos = Vector3f(0.0f, 0.0f, 0.0f);

	m_forward = Vector3f(0.0f, 0.0f, 1.0f);
	m_up = Vector3f(0.0f, 1.0f, 0.0f);
	m_right = Vector3f(1.0f, 0.0f, 0.0f);

    m_game = game;
}

void Camera::setPosition(float x, float y, float z)
{
    transformPos.x = x;
    transformPos.y = y;
    transformPos.z = z;
}

void Camera::setPosition(Vector3f& pos)
{
    transformPos = pos;
}

void Camera::onKeyboard(unsigned char key)
{
    switch (key) 
    {

    case GLUT_KEY_UP:
        transformPos += (cameraTransform.getZDirection() * m_speed);
        break;

    case GLUT_KEY_DOWN:
        transformPos -= (cameraTransform.getZDirection() * m_speed);
        break;

    case GLUT_KEY_RIGHT:
        transformPos += (cameraTransform.getXDirection() * m_speed);
        break;

    case GLUT_KEY_LEFT:
        transformPos -= (cameraTransform.getXDirection() * m_speed);
        break;

    case GLUT_KEY_PAGE_UP:
        transformPos += (cameraTransform.getYDirection() * m_speed);
        break;

    case GLUT_KEY_PAGE_DOWN:
        transformPos -= (cameraTransform.getYDirection() * m_speed);
        break;

    case 32:
        m_displayBoneIndex++;
        m_displayBoneIndex = m_displayBoneIndex % m_pSkinnedMesh->getNumBones();
        m_pLightingTech->setDisplayBoneIndex(m_displayBoneIndex);
        break;

    /*case 32:
        dynamicsWorld->debugDrawWorld();
        break;*/
    }
}

void Camera::onMouse(int x, int y)
{
    rotX += (y - (glutGet(GLUT_SCREEN_HEIGHT) / 2.0f))* 0.0008f;
    rotY += (x - (glutGet(GLUT_SCREEN_WIDTH) / 2.0f)) * 0.0008f;
   
    rotX = std::clamp(rotX, (float) - PI / 2.0f, (float)PI / 2.0f);

    glutWarpPointer(glutGet(GLUT_SCREEN_WIDTH) / 2.0f, glutGet(GLUT_SCREEN_HEIGHT) / 2.0f);
}

void Camera::setWeightColorDebug(SkinnedMesh* pMesh, LightingTechnique* lightTech)
{
    m_pSkinnedMesh = pMesh;
    m_pLightingTech = lightTech;
}

Matrix4f Camera::getMatrix()
{
    cameraTransform.setTranslation(transformPos);

    return cameraTransform;
}
