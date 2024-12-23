#include "plane.h"

Plane::Plane(const Vector3f& position, bool isRigidBody, btDiscreteDynamicsWorld* dynamicsWorld, float size)
{
    m_technique = new ObjectTechnique();
    if (!m_technique->init())
        exit(1);

    m_dynamicsWorld = dynamicsWorld;
    transform = position;
    setPosition(position);

    m_size = size;

    m_vertices.push_back(PlaneVert(Vector3f(0.5f, 0.0f, 0.5f) * size));  
    m_vertices.push_back(PlaneVert(Vector3f(-0.5f, 0.0f, 0.5f) * size)); 
    m_vertices.push_back(PlaneVert(Vector3f(-0.5f, 0.0f, -0.5f) * size)); 
    m_vertices.push_back(PlaneVert(Vector3f(0.5f, 0.0f, -0.5f) * size));

    // Define indices for two triangles forming a square plane
    m_indices = { 0, 1, 2, 0, 2, 3 };

    m_isRigidBody = isRigidBody;

    if (isRigidBody)
        createRigidBody();

    createGLState();
    populateBuffers();

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Plane::createRigidBody()
{
    btTransform planeTransform;
    planeTransform.setIdentity();  // Set to identity to start with no rotation
    planeTransform.setOrigin(btVector3(transform.x, transform.y, transform.z));  // Position the plane in world space

    btCollisionShape* groundShape = new btBoxShape(btVector3(0.5f * m_size, 0.001f, 0.5f * m_size));

    btScalar mass = 0.0f;
    btVector3 inertia(0, 0, 0);

    btDefaultMotionState* motionState = new btDefaultMotionState(planeTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, groundShape, inertia);
    m_rigidBody = new btRigidBody(rbInfo);

    m_rigidBody->setCollisionFlags(m_rigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
    m_dynamicsWorld->addRigidBody(m_rigidBody);
}

void Plane::updateRigidBody()
{
    btTransform worldTransform;
    m_objectTransform.setIdentity();

    m_rigidBody->getMotionState()->getWorldTransform(worldTransform);

    m_objectTransform.setTranslation(worldTransform.getOrigin()); 
    m_objectTransform.setQuaternion(worldTransform.getRotation()); 

}

void Plane::render(const Matrix4f& WVP)
{
    glBindVertexArray(m_VAO);
    glDisable(GL_CULL_FACE);

    m_technique->enable();
    m_technique->setWVP(WVP);

    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
}

void Plane::update(const Matrix4f& WVP)
{
    if (m_isRigidBody)
        updateRigidBody();

    render(WVP);
}

void Plane::createGLState()
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
    glVertexAttribPointer(POS_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(PlaneVert), (const void*)(numFloats * sizeof(float)));
    numFloats += 3;

}

void Plane::populateBuffers()
{
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices[0]) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices[0]) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW);
}

Matrix4f& Plane::getTransform()
{

    return m_objectTransform;
}

void Plane::setPosition(const Vector3f& position)
{
    m_objectTransform.setTranslation(position);
}
