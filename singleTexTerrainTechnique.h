#pragma once
#include "technique.h"
#include "lightingTechnique.h"
#include "math.h"

class SingleTexTerrainTechnique : public Technique
{
public:
    SingleTexTerrainTechnique();

    virtual bool init();

    void setWVP(const Matrix4f& VP);

    void setMinMaxHeight(float Min, float Max);

    void setTerrainTexture(int textureUnit);

    void setLightDirection(const DirectionalLight& dirLight);

    void setCamPosition(const Vector3f& camPos);
private:
    GLuint WVPLoc = -1;
    GLuint m_minHeightLoc = -1;
    GLuint m_maxHeightLoc = -1;
    GLuint m_terrainTexLoc = -1;
    GLuint m_lighDir = -1;
    GLuint m_camPos = -1;
};

