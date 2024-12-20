#pragma once
#include "math.h"
#include <GL/glew.h>
#include <vector>


// this header is included by baseTerrain.h so we have a forward 
// declaration for BaseTerrain.
class BaseTerrain;

using namespace std;

struct Vertex
{
public:
    Vector3f pos;
    Vector2f tex;
    Vector3f normal = Vector3f(0.0f, 0.0f, 0.0f);

public:
    Vertex() {}

    Vertex(float x, float y)
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

    void setVertex(const BaseTerrain* pTerrain, int x, int z);

};

class TriangleList
{
public:
	TriangleList();

	void createTriangleList(int width, int depth, const BaseTerrain* pTerrain);
	void render();

	void createGLState();
	void populateBuffers(const BaseTerrain* pTerrain);
	void initVertices(const BaseTerrain* pTerrain, vector<Vertex>& vertices);
	void initInidces(vector<unsigned int>& indices);
    void calcNormals(vector<unsigned int>& indices, vector<Vertex>& vertices);

private:
	int m_width = 0;
	int m_depth = 0;
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_IBO;

};

