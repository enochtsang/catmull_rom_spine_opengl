#pragma once

#include <vector>

#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShaderProgram.h"

class CatmullRomSpline {
public:
    CatmullRomSpline();
    ~CatmullRomSpline();
    bool initShaders();
    void addPoint(float x, float y);
    void clear();
    void render(glm::mat4 transform);

private:
    void loadPoints();

    std::vector<float> points_;
    std::vector<GLuint> ebo_;
    GLuint vao_id_;
    GLuint vbo_id_;
    GLuint ebo_id_;

    ShaderProgram tessShaderProgram_;
    ShaderProgram pointsShaderProgram_;
};
