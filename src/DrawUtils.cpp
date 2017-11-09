#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>

#include "DrawUtils.h"

void printGlVersion() {
    GLint major;
    GLint minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    std::cout << "OpenGL Version: " << major << "." << minor << std::endl;
}

void clearScreen() {
    glClearColor(1.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
}

