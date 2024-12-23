#pragma once
#include "../math.h"
#include "../objectTechnique.h"
#include <vector>
#include <GL/glew.h>
#include "../BulletPhysics/src/btBulletDynamicsCommon.h"

struct LightweightCubeVert
{
public:
    Vector3f pos;
    Vector3f normal = Vector3f(0.0f, 0.0f, 0.0f);
    Vector2f tex;

public:
    LightweightCubeVert() {}

    LightweightCubeVert(int x, int y, int z)
    {
        pos = Vector3f(x, y, z);
    }

    LightweightCubeVert(const Vector3f& vector)
    {
        pos = vector;
    }


    LightweightCubeVert(const Vector3f& vector, const Vector3f& norm)
    {
        pos = vector;
        normal = norm;
    }

    LightweightCubeVert(float x, float y)
    {
        pos = Vector3f(x, y, 0.0f);

    }

    void setVertex(int x, int y, int z)
    {
        pos = Vector3f(x, y, z);
    }

    void setVertex(int x, int z)
    {
        pos = Vector3f(x, 0.0f, z);
    }

    void setNormal(const Vector3f& norm)
    {
        normal = norm;
    }
};
class LightweightCube
{
public:
    LightweightCube(const Vector3f& position, bool isRigidBody, btDiscreteDynamicsWorld* dynamicsWorld, const Vector3f& lightDir);
    void createRigidBody();


    void updateRigidBody();
    void render(const Matrix4f& WVP, const Matrix4f& modelMatrix);
    void update(const Matrix4f& WVP, const Matrix4f& modelMatrix);

    void createGLState();
    void populateBuffers();
    Matrix4f& getTransform();
    Vector3f& getPosition() { return transform; }
    void setPosition(const Vector3f& position);

    Matrix4f world;
    Vector3f transform;

private:
    vector<LightweightCubeVert> m_vertices;
    vector<int> m_indices;

    Vector3f m_lightDir;

    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_IBO;

    ObjectTechnique* m_technique;

    bool m_isRigidBody;

    btRigidBody* m_rigidBody;
    Matrix4f m_objectTransform;

    btDiscreteDynamicsWorld* m_dynamicsWorld;
};

