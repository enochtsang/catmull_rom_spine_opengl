#pragma once

#include <string>

#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShaderProgram.h"

class Image {
public:
    Image();
    ~Image();
    bool initShaders();
    int width();
    int height();
    void toggleGrayscale();
    void toggleQuantization();
    bool setTexture(std::string path);
    void render(glm::mat4 transform);

private:
    float vertices_[20];
    GLuint vao_id_;
    GLuint vbo_id_;
    GLuint ebo_id_;
    GLuint texture_id_;

    ShaderProgram shaderProgram_;

    int width_;
    int height_;
    int grayscaled_;
    int quantized_;
};
