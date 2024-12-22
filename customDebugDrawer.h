#pragma once
#include <GL/glew.h>      
#include <GLFW/glfw3.h> 
#include "BulletPhysics/src/LinearMath/btIDebugDraw.h"
#include <iostream>

class CustomDebugDrawer : public btIDebugDraw {
public:
    int m_debugMode;

    CustomDebugDrawer() : m_debugMode(DBG_DrawWireframe) {}

    void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override 
    {
        // Use your rendering engine or OpenGL to draw a line
        glColor3f(2.0f, 2.0f, 2.0f);
        glBegin(GL_LINES);
        glVertex3f(from.x(), from.y(), from.z());
        glVertex3f(to.x(), to.y(), to.z());
        glEnd();
    }

    void drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override
    {
        std::cout << "Contact Point: " << pointOnB << std::endl;
        glColor3f(color.x(), color.y(), color.z());
        glBegin(GL_POINTS);
        glVertex3f(pointOnB.x(), pointOnB.y(), pointOnB.z());
        glEnd();
    }

    void reportErrorWarning(const char* warningString) override
    {
        std::cerr << "Bullet Warning: " << warningString << std::endl;
    }

    void draw3dText(const btVector3& location, const char* textString) override
    {
        // Render 3D text if supported by your engine
        std::cout << "3D Text: " << textString << " at " << location << std::endl;
    }

    void setDebugMode(int debugMode) override
    {
        m_debugMode = debugMode;
    }

    int getDebugMode() const override
    {
        return m_debugMode;
    }
};
