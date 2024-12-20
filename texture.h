#pragma once
#include <string>
#include <GL/glew.h>
#include "math.h"

class Texture
{
public:
	Texture(GLenum textureTarget, const std::string& fileName);
	Texture(GLenum textureTarget);

    bool load();
    void loadRaw(int width, int height, int BPP, const unsigned char* pImageData);
    

    void bind(GLenum textureUnit);
    void bindInternalDSA(GLenum textureUnit);

private:

    void loadInternal(const void* pImageData);
    void loadInternalDSA(const void* pImageData);

    std::string m_fileName;
    GLenum m_textureTarget;
    GLuint m_textureObj;
    int m_imageWidth = 0;
    int m_imageHeight = 0;
    int m_imageBPP = 0;
};

