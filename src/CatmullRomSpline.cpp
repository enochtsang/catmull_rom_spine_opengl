#include <iostream>

#include "CatmullRomSpline.h"

namespace {
    const GLuint POS_VAO_ID = 0;
    const GLuint STRIDE = 3;
}

CatmullRomSpline::CatmullRomSpline()
    : points_{}
    , ebo_{} {
    // initialize GPU objects
    glGenVertexArrays(1, &vao_id_);
    glGenBuffers(1, &vbo_id_);
    glGenBuffers(1, &ebo_id_);
    glBindVertexArray(vao_id_);

    // load empty points vector
    loadPoints();

    // position attribute
    glVertexAttribPointer(POS_VAO_ID, 3, GL_FLOAT, GL_FALSE,
                          STRIDE * sizeof(float), (void*)0);
    glEnableVertexAttribArray(POS_VAO_ID);

    // Determine max vertices in a patch
    GLint maxPatchVertices = 0;
    glGetIntegerv(GL_MAX_PATCH_VERTICES, &maxPatchVertices);
    std::cout << "Max supported patch vertices " << maxPatchVertices << std::endl;
}

CatmullRomSpline::~CatmullRomSpline() {
    glDeleteVertexArrays(1, &vao_id_);
    glDeleteBuffers(1, &vbo_id_);
    glDeleteBuffers(1, &ebo_id_);
}

bool CatmullRomSpline::initShaders() {
    if (tessShaderProgram_.linkShaders(
                "data/spline_vertex.glsl",
                "data/spline_tess_control.glsl",
                "data/spline_tess_eval.glsl",
                "data/spline_fragment.glsl")) {
        std::cout << "Spline tesselation shaders successfully initalized" << std::endl;
    }

    if (pointsShaderProgram_.linkShaders(
                "data/spline_vertex.glsl",
                "",
                "",
                "data/spline_fragment.glsl")) {
        std::cout << "Spline point shaders successfully initalized" << std::endl;
        return true;
    }

    std::cerr << "Image shaders failed to initialize" << std::endl;
    return false;
}

void CatmullRomSpline::addPoint(float x, float y) {
    points_.push_back(x);
    points_.push_back(y);
    points_.push_back(0.f); // z

    if (points_.size() <= 4 * 3) {
        // first patch
        ebo_.push_back(points_.size() / 3 - 1);
    } else {
        ebo_.push_back(points_.size() / 3 - 4);
        ebo_.push_back(points_.size() / 3 - 3);
        ebo_.push_back(points_.size() / 3 - 2);
        ebo_.push_back(points_.size() / 3 - 1);
    }

    loadPoints();
}

void CatmullRomSpline::render() {
    glBindVertexArray(vao_id_);

    // draw control points
    pointsShaderProgram_.use();
    glPointSize(7);
    glDrawArrays(GL_POINTS, 0, points_.size() / STRIDE);

    // draw tessallation
    tessShaderProgram_.use();
    glPatchParameteri(GL_PATCH_VERTICES, 4);
    glLineWidth(100.f);
    glDrawElements(GL_PATCHES, ebo_.size(), GL_UNSIGNED_INT, 0);
}

/* PRIVATE */

void CatmullRomSpline::loadPoints() {
    glBindVertexArray(vao_id_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);
    glBufferData(GL_ARRAY_BUFFER, points_.size() * sizeof(float),
                 &points_.front(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo_.size() * sizeof(GLuint),
                 &ebo_.front(), GL_STATIC_DRAW);
}
