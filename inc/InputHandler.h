#pragma once

#include <vector>

#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

class InputHandler {
public:
    InputHandler();
    ~InputHandler();

    void setKey(int key);
    void setMouseButtonLeftActive(bool active);
    std::vector<int> popKeys();
    bool mouseButtonLeftActive();

private:
    std::vector<int> keys_;
    bool mouseButtonLeftActive_;
};