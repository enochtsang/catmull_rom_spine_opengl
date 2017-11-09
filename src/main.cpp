#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <unistd.h>

#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

#include "DrawUtils.h"
#include "CatmullRomSpline.h"

std::vector<std::pair<float, float>> pointsToAdd;

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        // get screen dimensions
        int screenWidth = 0;
        int screenHeight = 0;
        glfwGetWindowSize(window, &screenWidth, &screenHeight);

        double newX;
        double newY;
        glfwGetCursorPos(window, &newX, &newY);
        std::cout << "Add point " << newX << " " << newY << std::endl;

        newX = (newX / (screenWidth / 2)) - 1;
        newY = -((newY / (screenHeight / 2)) - 1);

        pointsToAdd.push_back(std::make_pair(newX, newY));
    }
}

bool initWindow(int width, int height, GLFWwindow** window) {
    // initialize the GLFW windowing system
    if (!glfwInit()) {
        std::cerr << "ERROR: GLFW failed to initialize, TERMINATING" << std::endl;
        return false;
    }

    glfwSetErrorCallback([](int error, const char* description) {
        std::cerr << "GLFW ERROR " << error << ":" << std::endl;
        std::cerr << description << std::endl;
    });

    // Use OpenGL 4.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    *window = glfwCreateWindow(width, height, "Enoch Tsang - Catmull Rom Spline", 0, 0);

    if (!*window) {
        std::cout << "Program failed to create GLFW window, TERMINATING" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(*window);
    printGlVersion();
    return true;
}

int main(int argc, char* argv[]) {
    // Initialize window
    GLFWwindow* window = 0;

    if (!initWindow(800, 800, &window)) {
        std::cerr << "Initializing window failed, exiting..." << std::endl;
        return 1;
    }

    // Initialize spline
    CatmullRomSpline spline = CatmullRomSpline();

    if (!spline.initShaders()) {
        return 1;
    }

    // Handle adding points on mouse click
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    // main loop
    while (!glfwWindowShouldClose(window)) {
        for (auto point : pointsToAdd) {
            spline.addPoint(point.first, point.second);
        }

        pointsToAdd.clear();

        clearScreen();

        spline.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    std::cout << "Exiting" << std::endl;
    return 0;
}
