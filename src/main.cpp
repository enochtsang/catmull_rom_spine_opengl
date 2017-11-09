#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <unistd.h>

#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "DrawUtils.h"
#include "InputHandler.h"
#include "Image.h"
#include "CatmullRomSpline.h"

InputHandler inputHandler; // Global handler to handle key presses

void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        inputHandler.setKey(key); // edit global inputHandler
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        inputHandler.setMouseButtonLeftActive(true);
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        inputHandler.setMouseButtonLeftActive(false);
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
    *window = glfwCreateWindow(width, height, "Enoch Tsang - Image Manipulation", 0, 0);

    if (!*window) {
        std::cout << "Program failed to create GLFW window, TERMINATING" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(*window);
    printGlVersion();
    return true;
}

void adjustWindowSize(GLFWwindow* window, int width, int height) {
    int newWidth = width;
    int newHeight = height;

    auto videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    std::cout << "video mode width: " << videoMode->width << std::endl;
    std::cout << "video mode height: " << videoMode->height << std::endl;

    float ratio = float(width) / float(height);

    int maxWidth = videoMode->width * 0.8;

    if (newWidth >= maxWidth) {
        newWidth = maxWidth;
        newHeight = newWidth / ratio;
    }

    int maxHeight = videoMode->height * 0.8;

    if (newHeight >= maxHeight) {
        newHeight = maxHeight;
        newWidth = float(newHeight) * ratio;
    }

    std::cout << "max width: " << maxWidth << std::endl;
    std::cout << "max height: " << maxHeight << std::endl;

    glfwSetWindowSize(window, newWidth, newHeight);
    usleep(100000); // let window dimensions properly update

    int screenWidth = 0;
    int screenHeight = 0;
    glfwGetWindowSize(window, &screenWidth, &screenHeight);
    std::cout << "screen width: " << screenWidth << std::endl;
    std::cout << "screen height: " << screenHeight << std::endl;

    int fbWidth = 0;
    int fbHeight = 0;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    std::cout << "framebuffer width: " << fbWidth << std::endl;
    std::cout << "framebuffer height: " << fbHeight << std::endl;

    glViewport(0, 0,
               newWidth * (fbWidth / screenWidth),
               newHeight * (fbHeight / screenHeight));
}

int main(int argc, char* argv[]) {
    // Initialize window
    GLFWwindow* window = 0;

    if (!initWindow(200, 200, &window)) {
        std::cerr << "Initializing window failed, exiting..." << std::endl;
        return 1;
    }

    // Initalize image
    Image img = Image();

    if (argc != 2) {
        std::cerr << "Specify path as first argument" << std::endl;
        return 1;
    }

    if (!img.setTexture(argv[1])) {
        std::cerr << "Error setting texture: " << argv[1] << std::endl;
        return 1;
    }

    if (!img.initShaders()) {
        return 1;
    }

    std::cout << "image width: " << img.width() << std::endl;
    std::cout << "image height: " << img.height() << std::endl;

    // Readjust window to match image aspect ratio
    adjustWindowSize(window, img.width(), img.height());

    // Initialize spline
    CatmullRomSpline spline = CatmullRomSpline();

    if (!spline.initShaders()) {
        return 1;
    }

    // Set up input handle
    glfwSetKeyCallback(window, keyCallBack);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    // set up window transform
    float scale = 1.f;
    float translateX = 0.f;
    float translateY = 0.f;

    // set up vars to handle mouse panning
    double prevMousePosX = 0.0;
    double prevMousePosY = 0.0;
    bool mouseWasActive = false;
    glfwGetCursorPos(window, &prevMousePosX, &prevMousePosY);

    // get screen dimensions
    int screenWidth = 0;
    int screenHeight = 0;
    glfwGetWindowSize(window, &screenWidth, &screenHeight);

    // main loop
    while (!glfwWindowShouldClose(window)) {
        glfwSetCursor(window, glfwCreateStandardCursor(GLFW_ARROW_CURSOR));

        if (inputHandler.mouseButtonLeftActive()) {
            glfwSetCursor(window, glfwCreateStandardCursor(GLFW_HAND_CURSOR));

            if (mouseWasActive) { // active before and now
                double mousePosX = 0.0;
                double mousePosY = 0.0;
                glfwGetCursorPos(window, &mousePosX, &mousePosY);
                translateX += (mousePosX - prevMousePosX) / (screenWidth / 2 * scale);
                translateY += (prevMousePosY - mousePosY) / (screenHeight / 2 * scale);
            }
        }

        std::vector<int> keyEvents = inputHandler.popKeys(); // from global inputHandler

        for (int key : keyEvents) {
            switch (key) {
                case GLFW_KEY_G:
                    std::cout << "Gray scale toggled" << std::endl;
                    img.toggleGrayscale();
                    break;

                case GLFW_KEY_Q:
                    std::cout << "Quantization toggled" << std::endl;
                    img.toggleQuantization();
                    break;

                case GLFW_KEY_RIGHT_BRACKET:
                    std::cout << "Zoom in" << std::endl;
                    scale *= 1.2f;
                    break;

                case GLFW_KEY_LEFT_BRACKET:
                    std::cout << "Zoom out" << std::endl;
                    scale /= 1.2f;

                    if (scale < 0.1f) {
                        scale = 0.1f;
                    }

                    break;

                case GLFW_KEY_C:
                    std::cout << "Clear" << std::endl;
                    spline.clear();
                    break;

                case GLFW_KEY_A:
                    double newX;
                    double newY;
                    glfwGetCursorPos(window, &newX, &newY);
                    std::cout << "Add point " << newX << " " << newY << std::endl;

                    newX = (newX / (screenWidth / 2)) - 1;
                    newY = -((newY / (screenHeight / 2)) - 1);

                    glm::mat4 trans;
                    trans = glm::scale(trans, glm::vec3(scale, scale, scale));
                    trans = glm::translate(trans, glm::vec3(translateX, translateY, 0.0f));
                    trans = glm::inverse(trans);

                    glm::vec4 newPoint = trans * glm::vec4(newX, newY, 0.f, 1.f);

                    spline.addPoint(newPoint.x, newPoint.y);
                    break;
            }
        }

        glfwGetCursorPos(window, &prevMousePosX, &prevMousePosY);
        mouseWasActive = inputHandler.mouseButtonLeftActive();

        clearScreen();

        glm::mat4 trans;
        trans = glm::scale(trans, glm::vec3(scale, scale, scale));
        trans = glm::translate(trans, glm::vec3(translateX, translateY, 0.0f));

        img.render(trans);
        spline.render(trans);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    std::cout << "Exiting" << std::endl;
    return 0;
}
