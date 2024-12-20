#include <Windows.h>          // Windows-specific headers
#include <GL/glew.h>          // GLEW must come before OpenGL-related headers
#include <GLFW/glfw3.h>       // GLFW headers for windowing and input
#define GLFW_INCLUDE_NONE     // Prevent GLFW from including OpenGL headers
#include "imgui.h"            // Core ImGui headers
#include "imgui_impl_glfw.h"  // ImGui GLFW backend
#include "imgui_impl_opengl3.h" // ImGui OpenGL backend

class GLFW
{
    GLFWwindow* glfw_init(int major_ver, int minor_ver, int width, int height, bool is_full_screen, const char* title)
    {
        glfw_lib_init();

        GLFWmonitor* monitor = is_full_screen ? glfwGetPrimaryMonitor() : NULL;

        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

        if (major_ver > 0) {
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_ver);
        }

        if (minor_ver > 0) {
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_ver);
        }

        GLFWwindow* window = glfwCreateWindow(width, height, title, monitor, NULL);

        if (!window) {
            const char* pDesc = NULL;
            int error_code = glfwGetError(&pDesc);

            //OGLDEV_ERROR("Error creating window: %s", pDesc);
            exit(1);
        }

        glfwMakeContextCurrent(window);

        // The following functions must be called after the context is made current
        glGetIntegerv(GL_MAJOR_VERSION, &glMajorVersion);
        glGetIntegerv(GL_MINOR_VERSION, &glMinorVersion);

        if (major_ver > 0) {
            if (major_ver != glMajorVersion) {
                OGLDEV_ERROR("Requested major version %d is not the same as created version %d", major_ver, glMajorVersion);
                exit(0);
            }
        }

        if (minor_ver > 0) {
            if (minor_ver != glMinorVersion) {
                OGLDEV_ERROR("Requested minor version %d is not the same as created version %d", minor_ver, glMinorVersion);
                exit(0);
            }
        }

        // Must be done after glfw is initialized!
        init_glew();

        enable_debug_output();

        glfwSwapInterval(1);

        return window;
    }


};