#include "para-dars/core/InputManager.h"

GLFWwindow* InputManager::window = nullptr;

bool InputManager::mouseButtons[GLFW_MOUSE_BUTTON_LAST + 1] = { false };
bool InputManager::prevMouseButtons[GLFW_MOUSE_BUTTON_LAST + 1] = { false };

bool InputManager::keys[GLFW_KEY_LAST + 1] = { false };
bool InputManager::prevKeys[GLFW_KEY_LAST + 1] = { false };

glm::vec2 InputManager::mousePos(0.0f);
glm::vec2 InputManager::prevMousePos(0.0f);


void InputManager::Init(GLFWwindow* win) {
    window = win;
}

void InputManager::Update() {
    for (int i = 0; i <= GLFW_MOUSE_BUTTON_LAST; i++) {
        prevMouseButtons[i] = mouseButtons[i];
        mouseButtons[i] = glfwGetMouseButton(window, i) == GLFW_PRESS;
    }
    for (int i = 0; i <= GLFW_KEY_LAST; i++) {
        prevKeys[i] = keys[i];
        keys[i] = glfwGetKey(window, i) == GLFW_PRESS;
    }

    prevMousePos = mousePos;
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    mousePos = glm::vec2((float)x, (float)y);
}

bool InputManager::IsMouseButtonDown(int button) { 
    return mouseButtons[button]; 
}

bool InputManager::IsMouseButtonPressed(int button) {
    return mouseButtons[button] && !prevMouseButtons[button]; 
}

bool InputManager::IsMouseButtonReleased(int button) { 
    return !mouseButtons[button] && prevMouseButtons[button]; 
}

glm::vec2 InputManager::GetMousePosition() { 
    return mousePos; 
}

glm::vec2 InputManager::GetMouseDelta() { 
    return mousePos - prevMousePos; 
}

bool InputManager::IsKeyDown(int key) { 
    return keys[key]; 
}

bool InputManager::IsKeyPressed(int key) { 
    return keys[key] && !prevKeys[key]; 
}

bool InputManager::IsKeyReleased(int key) { 
    return !keys[key] && prevKeys[key]; 
}