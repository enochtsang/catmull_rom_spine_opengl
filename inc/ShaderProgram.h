#pragma once

#include <string>

#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

class ShaderProgram {
public:
    ShaderProgram();
    ~ShaderProgram();
    bool linkShaders(
        std::string vertexPath,
        std::string tessControlPath,
        std::string tessEvalPath,
        std::string fragmentPath);
    void use();
private:
    GLuint attachShader(std::string path, GLenum type, bool* success);
    GLuint id_;
};
