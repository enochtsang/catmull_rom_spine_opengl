#include <iostream>
#include <vector>

#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Image.h"

namespace {
    const GLuint POS_VAO_ID = 0;
    const GLuint TEX_VAO_ID = 1;
    const GLuint RECT_INDICES[] = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };
    const GLuint STRIDE = 5;
}

Image::Image()
// *INDENT-OFF*
: vertices_{
	// positions        // texture coords
	 1.f,  1.f, 0.0f, 1.0f, 1.0f,   // top right
	 1.f, -1.f, 0.0f, 1.0f, 0.0f,   // bottom right
	-1.f, -1.f, 0.0f, 0.0f, 0.0f,   // bottom left
	-1.f,  1.f, 0.0f, 0.0f, 1.0f    // top left
}
, grayscaled_(false)
, quantized_(false) {
// *INDENT-ON*

    // intialize GPU objects
    glGenVertexArrays(1, &vao_id_);
    glGenBuffers(1, &vbo_id_);
    glGenBuffers(1, &ebo_id_);
    glBindVertexArray(vao_id_);

    // initialize vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_),
                 vertices_, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(RECT_INDICES),
                 RECT_INDICES, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(POS_VAO_ID, 3, GL_FLOAT, GL_FALSE,
                          STRIDE* sizeof(float), (void*)0);
    glEnableVertexAttribArray(POS_VAO_ID);
    // texture attribute
    glVertexAttribPointer(TEX_VAO_ID, 2, GL_FLOAT, GL_FALSE,
                          STRIDE* sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(TEX_VAO_ID);

    // Initialize texture
    glGenTextures(1, &texture_id_);
    glBindTexture(GL_TEXTURE_2D, texture_id_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Image::~Image() {
    glDeleteVertexArrays(1, &vao_id_);
    glDeleteBuffers(1, &vbo_id_);
    glDeleteBuffers(1, &ebo_id_);
}

bool Image::initShaders() {
    if (shaderProgram_.linkShaders("data/img_vertex.glsl", "", "", "data/img_fragment.glsl")) {
        std::cout << "Image shaders successfully initalized" << std::endl;
        return true;
    }

    std::cerr << "Image shaders failed to initialize" << std::endl;
    return false;
}

int Image::width() {
    return width_;
}

int Image::height() {
    return height_;
}

void Image::toggleGrayscale() {
    if (grayscaled_ == 0) {
        grayscaled_ = 1;
    } else {
        grayscaled_ = 0;
    }
}

void Image::toggleQuantization() {
    if (quantized_ == 0) {
        quantized_ = 1;
    } else {
        quantized_ = 0;
    }
}

bool Image::setTexture(std::string path) {
    glBindTexture(GL_TEXTURE_2D, texture_id_);

    stbi_set_flip_vertically_on_load(true);
    int nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width_, &height_, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        // glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
        return false;
    }

    if (data) {
        stbi_image_free(data);
    }

    return true;
}

void Image::render(glm::mat4 transform) {
    glBindTexture(GL_TEXTURE_2D, texture_id_);
    shaderProgram_.use();
    int grayscaleLocation = glGetUniformLocation(shaderProgram_.id(), "grayscale");
    glUniform1i(grayscaleLocation, grayscaled_);
    int quantizeLocation = glGetUniformLocation(shaderProgram_.id(), "quantize");
    glUniform1i(quantizeLocation, quantized_);
    unsigned int transformLocation = glGetUniformLocation(shaderProgram_.id(), "transform");
    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));

    glBindVertexArray(vao_id_);

    glDrawElements(GL_TRIANGLES, sizeof(RECT_INDICES) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
}
