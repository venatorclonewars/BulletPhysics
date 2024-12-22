#pragma once
#include "math.h"
#include <GL/glew.h>
#include <vector>
#include "lodManager.h"
#include "frustumCulling.h"
#include "BulletPhysics/src/btBulletDynamicsCommon.h"

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

class GeomipGrid
{
public:
	GeomipGrid();
	~GeomipGrid();

	void createGeomipGrid(int width, int depth, int patchSize, const BaseTerrain* pTerrain, btDiscreteDynamicsWorld* dynamicsWorld);
	void render();
	void render(const Vector3f& cameraPos, const Matrix4f& WVP);
	void render(const Vector3f& cameraPos, Matrix4f& projection, Matrix4f& view, Matrix4f _view);

    void update(const Vector3f& cameraPos, Matrix4f& projection, Matrix4f& view, Matrix4f _view);
    void createRigidBody(vector<Vertex>& vertices);
    void updateRigidBody();

	void createGLState();
	void populateBuffers(const BaseTerrain* pTerrain);
	void initVertices(const BaseTerrain* pTerrain, vector<Vertex>& vertices);
	void initInidces(vector<unsigned int>& indices);
    int initIndices(vector<unsigned int>& indices);
    int initIndicesLOD(int index, std::vector<unsigned int>& indices, int lod);
    int initIndicesLODSingle(int index, std::vector<unsigned int>& indices, int lodCore, int lodLeft, int lodRight, int lodTop, int lodBottom);
	void calcNormals(vector<unsigned int>& indices, vector<Vertex>& vertices);
    unsigned int addTriangle(unsigned int index, vector<unsigned int>& indices, unsigned int v1, unsigned int v2, unsigned int v3);
    unsigned int createTriangleFan(int index, std::vector<unsigned int>& indices, int lodCore, int lodLeft, int lodRight, int lodTop, int lodBottom, int x, int z);

    int calcNumIndices();

    bool isPatchInsideViewFrustrum_ViewSpace(int x, int z, const Matrix4f& viewProj);
    bool isPointInsideViewFrustrum(const Vector3f& p, const Matrix4f& VP);

    bool isPatchInsideViewFrustum_WorldSpace(int x, int z, const FrustumCulling& fc);
private:
	int m_width = 0;
	int m_depth = 0;
	int m_patchSize = 0;
	int m_numPatchesX = 0;
	int m_numPatchesZ = 0;
    LodManager m_lodManager;

	int m_maxLOD = 0;
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_IBO;

    btDiscreteDynamicsWorld* m_dynamicsWorld;
    btConvexHullShape* m_convexShape;
    btTriangleMesh* m_triangleMesh;


    float m_worldScale = 1.0f;
    const BaseTerrain* m_pTerrain = NULL;

    struct SingleLodInfo
    {
        int start = 0;
        int count = 0;
    };

#define  LEFT 2
#define  RIGHT 2
#define  TOP 2
#define  BOTTOM 2

    struct LodInfo
    {
        SingleLodInfo info[LEFT][RIGHT][TOP][BOTTOM];
    };

    vector<LodInfo> m_lodInfo;
};

