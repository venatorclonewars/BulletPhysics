#pragma once
#include <cstdlib>
#include <cassert>
#include "math.h"
#include <string>
#include <random>

#define BULLET_ENABLE_DEBUG_DRAW
#include "BulletPhysics/src/LinearMath/btIDebugDraw.h"
#include "BulletPhysics/src/btBulletDynamicsCommon.h"

using namespace std;

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a) / sizeof((a)[0]))


/*struct Vertex
{
public:
    Vector3f pos;
    Vector3f color;
    Vector2f tex;

public:
    Vertex() {}

    Vertex(float x, float y)
    {
        pos = Vector3f(x, y, 0.0f);

        float red = (float)rand() / (float)RAND_MAX;
        float green = (float)rand() / (float)RAND_MAX;
        float blue = (float)rand() / (float)RAND_MAX;
        color = Vector3f(red, green, blue);
    }

    void setVertex(int x, int y, int z)
    {
        pos = Vector3f(x, y, z);
    }

    void setVertex(int x, int z)
    {
        pos = Vector3f(x, 0.0f, z);
    }

};*/

#define MAX_BONES_PER_VERTEX 4
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a) / sizeof((a)[0]))

struct VertexBoneData
{
    unsigned int boneIDs[MAX_BONES_PER_VERTEX] = { 0 };
    float weights[MAX_BONES_PER_VERTEX] = { 0.0f };

    VertexBoneData() {}

    void addBoneData(unsigned int boneID, float weight)
    {
        for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(boneIDs); i++)
        {
            if (weights[i] == 0.0f)
            {
                boneIDs[i] = boneID;
                weights[i] = weight;
                //printf("bone %d weight %f index %i\n", boneID, weight, i);
                return;
            }
        }

        assert(0);
    }


};

static float randomFloatRange(float min, float max) 
{
    random_device rd;  // Seed
    mt19937 gen(rd()); 
    uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}
