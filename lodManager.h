#pragma once
#include <vector>
#include "math.h"

using namespace std;
#define Z_FAR 500.0f

class LodManager
{
public:
	int initLodManager(int patchSize, int numPatchesX, int numPatchesZ, float worldScale);
    void update(const Vector3f& cameraPos);

    struct PatchLod
    {
        int Core = 0;
        int Left = 0;
        int Right = 0;
        int Top = 0;
        int Bottom = 0;
    };

    const PatchLod& getPatchLod(int patchX, int patchZ) const;

private:
    void calcLodRegions();
    void calcMaxLOD();
    void updateLodMapPass1(const Vector3f& cameraPos);
    void updateLodMapPass2(const Vector3f& cameraPos);

    int distanceToLod(float distance);


    int m_maxLOD = 0;
    int m_patchSize = 0;
    int m_numPatchesX = 0;
    int m_numPatchesZ = 0;
    float m_worldScale = 0.0f;

    Array2D<PatchLod> m_map;
    vector<int> m_regions;
};