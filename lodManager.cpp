#include "lodManager.h"

int LodManager::initLodManager(int patchSize, int numPatchesX, int numPatchesZ, float worldScale)
{
    m_patchSize = patchSize;
    m_numPatchesX = numPatchesX;
    m_numPatchesZ = numPatchesZ;
    m_worldScale = worldScale;

    calcMaxLOD();

    PatchLod Zero;
    m_map.setArray2D(numPatchesX, numPatchesZ, Zero);

    m_regions.resize(m_maxLOD + 1);

    calcLodRegions();

    return m_maxLOD;
}

void LodManager::update(const Vector3f& cameraPos)
{
    updateLodMapPass1(cameraPos);
    updateLodMapPass2(cameraPos);
}

const LodManager::PatchLod& LodManager::getPatchLod(int patchX, int patchZ) const
{
   return m_map.get(patchX, patchZ);
}

void LodManager::calcLodRegions()
{
    int Sum = 0;

    for (int i = 0; i <= m_maxLOD; i++) {
        Sum += (i + 1);
    }

    printf("Sum %d\n", Sum);

    float X = Z_FAR / (float)Sum;

    int Temp = 0;

    for (int i = 0; i <= m_maxLOD; i++) {
        int CurRange = (int)(X * (i + 1));
        m_regions[i] = Temp + CurRange;
        Temp += CurRange;
        printf("%d %d\n", i, m_regions[i]);
    }
}

void LodManager::calcMaxLOD()
{
    int NumSegments = m_patchSize - 1;
    if (ceilf(log2f((float)NumSegments)) != floorf(log2f((float)NumSegments))) {
        printf("The number of vertices in the patch minus one must be a power of two\n");
        printf("%f %f\n", ceilf(log2f((float)NumSegments)), floorf(log2f((float)NumSegments)));
        exit(0);
    }

    int patchSizeLog2 = (int)log2f((float)NumSegments);
    printf("log2 of patch size %d is %d\n", m_patchSize, patchSizeLog2);
    m_maxLOD = patchSizeLog2 - 1;
}

void LodManager::updateLodMapPass1(const Vector3f& cameraPos)
{
    int centerStep = m_patchSize / 2;

    for (int LodMapZ = 0; LodMapZ < m_numPatchesZ; LodMapZ++) {
        for (int LodMapX = 0; LodMapX < m_numPatchesX; LodMapX++) {
            int x = LodMapX * (m_patchSize - 1) + centerStep;
            int z = LodMapZ * (m_patchSize - 1) + centerStep;

            Vector3f PatchCenter = Vector3f(x * (float)m_worldScale, 0.0f, z * (float)m_worldScale);

            float DistanceToCamera = cameraPos.distance(PatchCenter);
            /*printf("distance to camera %f \n", DistanceToCamera);
            printf("patch center %f %f %f \n", PatchCenter.x, PatchCenter.y, PatchCenter.z);
            printf("camera %f %f %f \n", cameraPos.x, cameraPos.y, cameraPos.z);*/
            int CoreLod = distanceToLod(DistanceToCamera);

            PatchLod* pPatchLOD = m_map.getAddr(LodMapX, LodMapZ);
            pPatchLOD->Core = CoreLod;
        }
    }
}

void LodManager::updateLodMapPass2(const Vector3f& cameraPos)
{
    int Step = m_patchSize / 2;

    for (int LodMapZ = 0; LodMapZ < m_numPatchesZ; LodMapZ++) {
        for (int LodMapX = 0; LodMapX < m_numPatchesX; LodMapX++) {
            int CoreLod = m_map.get(LodMapX, LodMapZ).Core;

            int IndexLeft = LodMapX;
            int IndexRight = LodMapX;
            int IndexTop = LodMapZ;
            int IndexBottom = LodMapZ;

            if (LodMapX > 0) {
                IndexLeft--;

                if (m_map.get(IndexLeft, LodMapZ).Core > CoreLod) {
                    m_map.at(LodMapX, LodMapZ).Left = 1;
                }
                else {
                    m_map.at(LodMapX, LodMapZ).Left = 0;
                }
            }

            if (LodMapX < m_numPatchesX - 1) {
                IndexRight++;

                if (m_map.get(IndexRight, LodMapZ).Core > CoreLod) {
                    m_map.at(LodMapX, LodMapZ).Right = 1;
                }
                else {
                    m_map.at(LodMapX, LodMapZ).Right = 0;
                }
            }

            if (LodMapZ > 0) {
                IndexBottom--;

                if (m_map.get(LodMapX, IndexBottom).Core > CoreLod) {
                    m_map.at(LodMapX, LodMapZ).Bottom = 1;
                }
                else {
                    m_map.at(LodMapX, LodMapZ).Bottom = 0;
                }
            }

            if (LodMapZ < m_numPatchesZ - 1) {
                IndexTop++;

                if (m_map.get(LodMapX, IndexTop).Core > CoreLod) {
                    m_map.at(LodMapX, LodMapZ).Top = 1;
                }
                else {
                    m_map.at(LodMapX, LodMapZ).Top = 0;
                }
            }
        }
    }
}

int LodManager::distanceToLod(float distance)
{
    int lod = m_maxLOD;

    for (int i = 0; i <= m_maxLOD; i++) {
        if (distance < m_regions[i]) {
            lod = i;
            break;
        }
    }

    return lod;
}
