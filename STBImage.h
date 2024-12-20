#pragma once
#include "stb_image.h"
#include "math.h"
#include <cassert>

class STBImage
{
public:
    void load(const char* pFilename)
    {
        
        stbi_set_flip_vertically_on_load(1);

        m_imageData = stbi_load(pFilename, &m_width, &m_height, &m_bpp, 0);

        if (!m_imageData) 
        {
            printf("Can't load texture from '%s' - %s\n", pFilename, stbi_failure_reason());
            exit(0);
        }

        printf("Loaded '%s' - width %d, height %d, bpp %d\n", pFilename, m_width, m_height, m_bpp);
    }

    Vector3f getColor(int x, int y)
    {
        assert(m_width > 0);
        assert(m_height > 0);

        int wrappedX = x % m_width;
        int wrappedY = y % m_height;

        Vector3f color;

        unsigned char* p = m_imageData + (wrappedY * m_width + wrappedX) * m_bpp;
        color.x = (float)p[0];
        color.y = (float)p[1];
        color.z = (float)p[2];

        return color;
    }

private:
    int m_width = 0;
    int m_height = 0;
    int m_bpp = 0;
    unsigned char* m_imageData = NULL;
};
