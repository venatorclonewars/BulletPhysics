#pragma once
#include "../math.h"
#include "../objectTechnique.h"
#include <vector>
#include <GL/glew.h>
#include "../BulletPhysics/src/btBulletDynamicsCommon.h"

struct PlaneVert
{
public:
    Vector3f pos;
    Vector2f tex;
    Vector3f normal = Vector3f(0.0f, 0.0f, 0.0f);

public:
    PlaneVert() {}

    PlaneVert(int x, int y, int z)
    {
        pos = Vector3f(x, y, z);
    }

    PlaneVert(const Vector3f& vector)
    {
        pos = vector;
    }

    PlaneVert(float x, float y)
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
};
class Plane
{
public:
    Plane(const Vector3f& position, bool isRigidBody, btDiscreteDynamicsWorld* dynamicsWorld, float size);
    void createRigidBody();


    void updateRigidBody();
    void render(const Matrix4f& WVP);
    void update(const Matrix4f& WVP);

    void createGLState();
    void populateBuffers();
    Matrix4f& getTransform();
    Vector3f& getPosition() { return transform; }
    void setPosition(const Vector3f& position);

    Matrix4f world;
    Vector3f transform;

private:
    vector<PlaneVert> m_vertices;
    vector<int> m_indices;

    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_IBO;

    ObjectTechnique* m_technique;

    bool m_isRigidBody;
    float m_size;

    btRigidBody* m_rigidBody;
    Matrix4f m_objectTransform;

    btDiscreteDynamicsWorld* m_dynamicsWorld;
};

