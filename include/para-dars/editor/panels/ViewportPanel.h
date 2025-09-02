#pragma once

#include "para-dars/editor/Panel.h"
#include "para-dars/ecs/systems/RenderSystem.h"
#include "para-dars/graphics/Shader.h"
#include "para-dars/graphics/FullscreenQuad.h"

class ViewportPanel : public Panel {
    public: 
        ViewportPanel(Editor* editor, RenderSystem* renderSystem);

        void OnImGuiRender() override;

        void Resize(int width, int height);

        Camera GetCamera();

    private:
        RenderSystem* renderSystem;
        int viewportWidth;
        int viewportHeight;
        FullscreenQuad fullscreenQuad;

        Framebuffer maskFBO;
        Framebuffer postFBO;
        Framebuffer idEncodingFBO;
        Shader maskShader;
        Shader outlinePostShader;
        Shader idEncodingShader;

        void CameraInput();
        void HandleResize(ImVec2 availableSize);
        void MainRender(ImVec2 availableSize, Scene* scene, EntityID selected, Camera camera, glm::mat4 view, glm::mat4 projection);
        void EncodeIDBuffer(Scene* scene, glm::mat4 view, glm::mat4 projection);
        void DrawImGuizmo(Scene* scene, EntityID selected, glm::mat4 view, glm::mat4 projection);
        void SelectionInput(ImVec2 availableSize);

        unsigned int selectionFramebuffer, selectionColorAttachment, selectionDepthAttachment;
        glm::vec3 focalPoint = glm::vec3(0.0f);
        float distance = 10.0f;
        float yaw = glm::radians(0.0f);
        float pitch = glm::radians(30.0f);
        float orbitSpeed = 0.003f;
        float panSpeed = 0.004f;
        float zoomSpeed = 0.5f;

        void OnCameraOrbit(glm::vec2 delta);
        void OnCameraPan(glm::vec2 delta);
        void OnCameraZoom(float delta);
        glm::vec3 CalculateCameraPosition();
};