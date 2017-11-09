#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "ShaderProgram.h"

ShaderProgram::ShaderProgram()
    : id_(glCreateProgram()) {
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(id_);
}

bool ShaderProgram::linkShaders(
    std::string vertexPath,
    std::string tessControlPath,
    std::string tessEvalPath,
    std::string fragmentPath) {
    use();

    std::vector<GLuint> shaderIds{}; // for cleanup

    // Compile and attach shaders
    bool success;
    shaderIds.push_back(attachShader(vertexPath, GL_VERTEX_SHADER, &success));

    if (!success) {
        return false;
    }

    if (tessControlPath != "") {
        shaderIds.push_back(attachShader(tessControlPath, GL_TESS_CONTROL_SHADER, &success));

        if (!success) {
            return false;
        }
    }

    if (tessEvalPath != "") {
        shaderIds.push_back(attachShader(tessEvalPath, GL_TESS_EVALUATION_SHADER, &success));

        if (!success) {
            return false;
        }
    }

    shaderIds.push_back(attachShader(fragmentPath, GL_FRAGMENT_SHADER, &success));

    if (!success) {
        return false;
    }

    // Link shaders
    glLinkProgram(id_);
    GLint status;
    glGetProgramiv(id_, GL_LINK_STATUS, &status);

    if (status == GL_FALSE) {
        GLint length;
        glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &length);
        std::string info(length, ' ');
        glGetProgramInfoLog(id_, info.length(), &length, &info[0]);

        std::cerr << "ERROR linking program:" << std::endl;
        std::cerr << info << std::endl;
        return false;
    } else {
        std::cout << "Successfully linked shaders" << std::endl;
    }

    // Cleanup shaders after cleanup
    for (auto shaderId : shaderIds) {
        glDeleteShader(shaderId);
    }

    return true;
}

void ShaderProgram::use() {
    glUseProgram(id_);
}

/* PRIVATE */

// return shader id
GLuint ShaderProgram::attachShader(std::string path, GLenum type, bool* success) {
    // Load in shader file
    std::ifstream in(path);
    std::string buffer = [&in] {
        std::ostringstream ss {};
        ss << in.rdbuf();
        return ss.str();
    }();
    const char* buffer_array[] = { buffer.c_str() };

    // Load shader into OpenGL
    GLuint shaderId = glCreateShader(type);

    glShaderSource(shaderId, 1, buffer_array, 0);
    glCompileShader(shaderId);

    GLint status;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);

    // Error check
    if (status == GL_FALSE) {
        GLint length;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
        std::string info(length, ' ');
        glGetShaderInfoLog(shaderId, info.length(), &length, &info[0]);
        std::cerr << "ERROR compiling shader:" << std::endl;
        std::cerr << info << std::endl;
        *success = false;
        return shaderId;
    }

    glAttachShader(id_, shaderId);
    std::cout << "Successfully compiled and attached shader " << path << std::endl;
    *success = true;
    return shaderId;
}


