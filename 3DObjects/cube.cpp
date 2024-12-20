#include "cube.h"

Cube::Cube(const Vector3f& position)
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

    createGLState();
    populateBuffers();

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Cube::render(const Matrix4f& WVP)
{
    glBindVertexArray(m_VAO);

    m_technique->enable();
    m_technique->setWVP(WVP);

    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
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
    return world;
}

void Cube::setPosition(const Vector3f& position)
{
    world.setTranslation(position);
}
