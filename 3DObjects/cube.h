#pragma once
#include "../math.h"
#include "../objectTechnique.h"
#include <vector>
#include <GL/glew.h>
#include "../BulletPhysics/src/btBulletDynamicsCommon.h"

struct Vert
{
public:
    Vector3f pos;
    Vector2f tex;
    Vector3f normal = Vector3f(0.0f, 0.0f, 0.0f);

public:
    Vert() {}

    Vert(int x, int y, int z)
    {
        pos = Vector3f(x, y, z);
    }

    Vert(const Vector3f& vector)
    {
        pos = vector;
    }

    Vert(float x, float y)
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
class Cube
{
public:
	Cube(const Vector3f& position, bool isRigidBody, btDiscreteDynamicsWorld* dynamicsWorld);
    void createRigidBody(btDiscreteDynamicsWorld* dynamicsWorld);


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
    vector<Vert> m_vertices;
    vector<int> m_indices;

    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_IBO;

    ObjectTechnique* m_technique;

    bool m_isRigidBody;

    btRigidBody* m_rigidBody;
    Matrix4f m_objectTransform;
};

