#pragma once
#include <GL/glew.h>
#include <string>

static bool isGLVersionHigher(int major, int minor) 
{
    const char* versionString = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    if (!versionString) {
        return false; // Failed to retrieve the version
    }

    int currentMajor = 0, currentMinor = 0;
    sscanf_s(versionString, "%d.%d", &currentMajor, &currentMinor);

    if (currentMajor > major) {
        return true;
    }
    else if (currentMajor == major && currentMinor >= minor) {
        return true;
    }
    return false;
}
