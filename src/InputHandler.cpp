#include "InputHandler.h"

InputHandler::InputHandler() {}

InputHandler::~InputHandler() {}

void InputHandler::setKey(int key) {
    keys_.push_back(key);
}

void InputHandler::setMouseButtonLeftActive(bool active) {
    mouseButtonLeftActive_ = active;
}

bool InputHandler::mouseButtonLeftActive() {
    return mouseButtonLeftActive_;
}

std::vector<int> InputHandler::popKeys() {
    auto poppedKeys = keys_;
    keys_ = std::vector<int>();
    return poppedKeys;
}

