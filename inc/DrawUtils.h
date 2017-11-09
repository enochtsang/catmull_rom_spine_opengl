#pragma once
#include <string>

#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

void printGlVersion();

// Prints to stderr any errors
bool compileShader(const std::string& path, GLuint type, GLuint* shaderId);
// Prints to stderr any errors
bool linkShaderProgram(GLuint programId);

// Draw black over the whole screen
void clearScreen();
