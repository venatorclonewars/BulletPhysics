#include "cube.h"

Cube::Cube(const Vector3f& position, bool isRigidBody, btDiscreteDynamicsWorld* dynamicsWorld)
{
    m_technique = new ObjectTechnique();
    if (!m_technique->init())
        exit(1);

    transform = position;
    setPosition(position);

    m_vertices.push_back(Vert(Vector3f(0.5f, 0.5f, 0.5f)));
    m_vertices.push_back(Vert(Vector3f(-0.5f, 0.5f, -0.5f)));
    m_vertices.push_back(Vert(Vector3f(-0.5f, 0.5f, 0.5f)));
    m_vertices.push_back((Vector3f(0.5f, -0.5f, -0.5f)));
    m_vertices.push_back((Vector3f(-0.5f, -0.5f, -0.5f)));
    m_vertices.push_back((Vector3f(0.5f, 0.5f, -0.5f)));
    m_vertices.push_back((Vector3f(0.5f, -0.5f, 0.5f)));
    m_vertices.push_back((Vector3f(-0.5f, -0.5f, 0.5f)));

    m_indices = {
        0, 1, 2,
        1, 3, 4,
        5, 6, 3,
        7, 3, 6,
        2, 4, 7,
        0, 7, 6,
        0, 5, 1,
        1, 5, 3,
        5, 0, 6,
        7, 4, 3,
        2, 1, 4,
        0, 2, 7
    };

    m_isRigidBody = isRigidBody;

    if (isRigidBody)
        createRigidBody(dynamicsWorld);

    createGLState();
    populateBuffers();

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Cube::createRigidBody(btDiscreteDynamicsWorld* dynamicsWorld)
{
    btCollisionShape* fallShape = new btBoxShape(btVector3(0.5, 0.5, 0.5));  // Cube of size 2x2x2
    btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(transform.x, transform.y, transform.z)));  // Initial position (0, 10, 0)
    btScalar mass = 1.0f;

    btVector3 fallInertia(0, 0, 0);
    fallShape->calculateLocalInertia(mass, fallInertia);

    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
    m_rigidBody = new btRigidBody(fallRigidBodyCI);
    dynamicsWorld->addRigidBody(m_rigidBody);
}

void Cube::updateRigidBody()
{
    btTransform worldTransform;
    m_objectTransform.setIdentity();

    m_rigidBody->getMotionState()->getWorldTransform(worldTransform);

    m_objectTransform.setTranslation(worldTransform.getOrigin());  // Set the translation part (position)
    m_objectTransform.setRotationFromQuaternion(worldTransform.getRotation());  // Set the rotation part from quaternion

}

void Cube::render(const Matrix4f& WVP)
{
    glBindVertexArray(m_VAO);

    m_technique->enable();
    m_technique->setWVP(WVP);

    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void Cube::update(const Matrix4f& WVP)
{
    if (m_isRigidBody)
        updateRigidBody();

    render(WVP);
}

void Cube::createGLState()
{
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    glGenBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

    int POS_LOC = 0;

    size_t numFloats = 0;

    glEnableVertexAttribArray(POS_LOC);
    glVertexAttribPointer(POS_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(Vert), (const void*)(numFloats * sizeof(float)));
    numFloats += 3;

}

void Cube::populateBuffers()
{
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices[0]) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices[0]) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW);
}

Matrix4f& Cube::getTransform()
{

    return m_objectTransform;
}

void Cube::setPosition(const Vector3f& position)
{
    m_objectTransform.setTranslation(position);
}
