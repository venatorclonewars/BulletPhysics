#include "lightweightCube.h"

LightweightCube::LightweightCube(const Vector3f& position, bool isRigidBody, btDiscreteDynamicsWorld* dynamicsWorld, const Vector3f& lightDir)
{
    m_technique = new ObjectTechnique();
    if (!m_technique->init())
        exit(1);

    m_technique->setLightDir(lightDir);
    m_lightDir = lightDir;

    m_dynamicsWorld = dynamicsWorld;
    transform = position;
    setPosition(position);

    // Lightweight version - bad for lighting
    m_vertices.push_back(LightweightCubeVert(Vector3f(0.5f, 0.5f, 0.5f)));
    m_vertices.push_back(LightweightCubeVert(Vector3f(-0.5f, 0.5f, -0.5f)));
    m_vertices.push_back(LightweightCubeVert(Vector3f(-0.5f, 0.5f, 0.5f)));
    m_vertices.push_back(LightweightCubeVert(Vector3f(0.5f, -0.5f, -0.5f)));
    m_vertices.push_back(LightweightCubeVert(Vector3f(-0.5f, -0.5f, -0.5f)));
    m_vertices.push_back(LightweightCubeVert(Vector3f(0.5f, 0.5f, -0.5f)));
    m_vertices.push_back(LightweightCubeVert(Vector3f(0.5f, -0.5f, 0.5f)));
    m_vertices.push_back(LightweightCubeVert(Vector3f(-0.5f, -0.5f, 0.5f)));



     m_indices =
     {
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

     // Calculate normals for each vertex
     for (unsigned int i = 0; i < m_indices.size(); i += 3)
     {
         // Retrieve the three vertices that form the current triangle
         int index0 = m_indices[i];
         int index1 = m_indices[i + 1];
         int index2 = m_indices[i + 2];

         // Get the vertices from m_vertices
         Vector3f v0 = m_vertices[index0].pos;
         Vector3f v1 = m_vertices[index1].pos;
         Vector3f v2 = m_vertices[index2].pos;

         // Calculate two edge vectors of the triangle
         Vector3f edge1 = v1 - v0;
         Vector3f edge2 = v2 - v0;

         // Compute the normal of the triangle using the cross product
         Vector3f normal = edge1.cross(edge2);
         normal = normal.normalize();  // Normalize the normal

         // Add the normal to the normals of each vertex of the triangle
         m_vertices[index0].normal += normal;
         m_vertices[index1].normal += normal;
         m_vertices[index2].normal += normal;
     }

     // Normalize all accumulated normals
     for (auto& vertex : m_vertices)
     {
         vertex.normal = vertex.normal.normalize();  // Normalize each vertex normal
     }

    m_isRigidBody = isRigidBody;

    if (isRigidBody)
        createRigidBody();

    createGLState();
    populateBuffers();

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void LightweightCube::createRigidBody()
{
    btCollisionShape* fallShape = new btBoxShape(btVector3(0.5, 0.5, 0.5));  // Cube of size 2x2x2
    btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(transform.x, transform.y, transform.z)));  // Initial position (0, 10, 0)
    btScalar mass = 1.0f;

    btVector3 fallInertia(0, 0, 0);
    fallShape->calculateLocalInertia(mass, fallInertia);

    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
    m_rigidBody = new btRigidBody(fallRigidBodyCI);
    m_dynamicsWorld->addRigidBody(m_rigidBody);
}

void LightweightCube::updateRigidBody()
{
    btTransform worldTransform;
    m_objectTransform.setIdentity();

    m_rigidBody->getMotionState()->getWorldTransform(worldTransform);

    m_objectTransform.setTranslation(worldTransform.getOrigin());
    m_objectTransform.setQuaternion(worldTransform.getRotation());

}

void LightweightCube::render(const Matrix4f& WVP, const Matrix4f& modelMatrix)
{
    glBindVertexArray(m_VAO);
   
    m_technique->enable();
    m_technique->setWVP(WVP);
    m_technique->setLightDir(m_lightDir);
    m_technique->setModelMatrix(modelMatrix);

    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void LightweightCube::update(const Matrix4f& WVP, const Matrix4f& modelMatrix)
{
    if (m_isRigidBody)
        updateRigidBody();

    render(WVP, modelMatrix);
}

void LightweightCube::createGLState()
{
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    glGenBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

    int POS_LOC = 0;
    int NORM_LOC = 1;

    size_t numFloats = 0;

    glEnableVertexAttribArray(POS_LOC);
    glVertexAttribPointer(POS_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(LightweightCubeVert), (const void*)(numFloats * sizeof(float)));
    numFloats += 3;
    glEnableVertexAttribArray(NORM_LOC);
    glVertexAttribPointer(NORM_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(LightweightCubeVert), (const void*)(numFloats * sizeof(float)));
    numFloats += 3;

}

void LightweightCube::populateBuffers()
{
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices[0]) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices[0]) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW);
}

Matrix4f& LightweightCube::getTransform()
{

    return m_objectTransform;
}

void LightweightCube::setPosition(const Vector3f& position)
{
    m_objectTransform.setTranslation(position);
}
