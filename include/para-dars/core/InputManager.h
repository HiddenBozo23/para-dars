#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <unordered_map>

class InputManager {
    public:
        static void Init(GLFWwindow* window);
        static void Update();

        static bool IsMouseButtonDown(int button);
        static bool IsMouseButtonPressed(int button);
        static bool IsMouseButtonReleased(int button);

        static glm::vec2 GetMousePosition();
        static glm::vec2 GetMouseDelta();
        static glm::vec2 GetScrollDelta();
        static void ResetScrollDelta();

        static bool IsKeyDown(int key);
        static bool IsKeyPressed(int key);
        static bool IsKeyReleased(int key);

    private:
        static GLFWwindow* window;

        static bool mouseButtons[GLFW_MOUSE_BUTTON_LAST + 1];
        static bool prevMouseButtons[GLFW_MOUSE_BUTTON_LAST + 1];

        static bool keys[GLFW_KEY_LAST + 1];
        static bool prevKeys[GLFW_KEY_LAST + 1];

        static glm::vec2 mousePos;
        static glm::vec2 prevMousePos;
        static glm::vec2 scrollDelta;

        static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};