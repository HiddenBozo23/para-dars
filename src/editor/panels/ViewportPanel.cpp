#include <imgui.h>
#include <imgui_internal.h>
#include <imguizmo.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "para-dars/editor/panels/ViewportPanel.h"
#include "para-dars/ecs/components/ModelComponent.h"
#include "para-dars/core/InputManager.h"

ViewportPanel::ViewportPanel(Editor* editor, RenderSystem* renderSystem)
    : Panel(editor), renderSystem(renderSystem), 
    maskShader(Shader("shaders/mask.vert", "shaders/mask.frag")),  
    outlinePostShader(Shader("shaders/outline.vert", "shaders/outline.frag")), 
    idEncodingShader(Shader("shaders/mask.vert", "shaders/idEncoding.frag")) {}

void ViewportPanel::OnImGuiRender() {
    ImGui::Begin("Viewport");

    ImVec2 availableSize = ImGui::GetContentRegionAvail();
    Scene* scene = renderSystem->GetScene();
    EntityID selected = GetEditor()->GetSelectedEntity();
    Camera camera = GetCamera();
    glm::mat4 view = glm::inverse(camera.transform.GetMatrix());
    glm::mat4 projection = camera.camera.GetProjectionMatrix(static_cast<float>(viewportWidth) / viewportHeight);

    HandleResize(availableSize);
    MainRender(availableSize, scene, selected, camera, view, projection);
    EncodeIDBuffer(scene, view, projection);
    DrawImGuizmo(scene, selected, view, projection);
    CameraInput(scene, selected);
    SelectionInput(availableSize);    
        
    ImGui::End();
}

void ViewportPanel::Resize(int width, int height) {
    renderSystem->ResizeFrameBuffer(width, height);
    maskFBO.Resize(width, height);
    postFBO.Resize(width, height);
    idEncodingFBO.Resize(width, height);
}

Camera ViewportPanel::GetCamera() {
    Camera cam;

    cam.transform.position = CalculateCameraPosition();
    cam.transform.rotation = glm::vec3(pitch, yaw, 0.0f);
    cam.transform.scale = glm::vec3(1.0f);

    cam.camera.fov = 45.0f;
    cam.camera.nearPlane = 0.1f;
    cam.camera.farPlane = 100.0f;

    return cam;
}

void ViewportPanel::HandleResize(ImVec2 availableSize) {
    if (viewportWidth != static_cast<float>(availableSize.x) || viewportHeight != static_cast<float>(availableSize.y)) {
        viewportWidth = static_cast<float>(availableSize.x);
        viewportHeight = static_cast<float>(availableSize.y);
        Resize(viewportWidth, viewportHeight);
    }
}

void ViewportPanel::MainRender(ImVec2 availableSize, Scene* scene, EntityID selected, Camera camera, glm::mat4 view, glm::mat4 projection) {
    renderSystem->Render(camera);

    maskFBO.Bind();
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (selected != INVALID_ENTITY && scene->HasComponent<ModelComponent>(selected) && scene->HasComponent<TransformComponent>(selected)) {
        maskShader.Use();
        maskShader.SetMat4("model", scene->GetComponent<TransformComponent>(selected)->GetMatrix());
        maskShader.SetMat4("view", view);
        maskShader.SetMat4("projection", projection);
        scene->GetComponent<ModelComponent>(selected)->model->Draw(maskShader, false);
    }

    postFBO.Bind();
    outlinePostShader.Use();
    outlinePostShader.SetInt("scene", 0);
    outlinePostShader.SetInt("mask", 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderSystem->GetFramebufferTexture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, maskFBO.GetColorAttachment());

    fullscreenQuad.Draw();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(0);

    ImGui::Image(postFBO.GetColorAttachment(), availableSize, ImVec2(0, 1), ImVec2(1, 0));
}

void ViewportPanel::EncodeIDBuffer(Scene* scene, glm::mat4 view, glm::mat4 projection) {
    idEncodingFBO.Bind();
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    idEncodingShader.Use();
    idEncodingShader.SetMat4("view", view);
    idEncodingShader.SetMat4("projection", projection);
    for (EntityID entity : renderSystem->GetTrackedEntities()) {
        if (!scene->GetComponent<ModelComponent>(entity)->model) continue;
        idEncodingShader.SetInt("entityID", (int)entity);
        idEncodingShader.SetMat4("model", scene->GetComponent<TransformComponent>(entity)->GetMatrix());
        scene->GetComponent<ModelComponent>(entity)->model->Draw(idEncodingShader, false);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ViewportPanel::DrawImGuizmo(Scene* scene, EntityID selected, glm::mat4 view, glm::mat4 projection) {
    if (selected != INVALID_ENTITY && scene->HasComponent<TransformComponent>(selected)) {
        ImGuizmo::BeginFrame();
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());

        ImVec2 imageMin = ImGui::GetItemRectMin();
        ImVec2 imageMax = ImGui::GetItemRectMax();
        ImGuizmo::SetRect(imageMin.x, imageMin.y, imageMax.x - imageMin.x, imageMax.y - imageMin.y);
        glm::mat4 model = scene->GetComponent<TransformComponent>(selected)->GetMatrix();

        ImGuizmo::Manipulate(
            glm::value_ptr(view),
            glm::value_ptr(projection),
            ImGuizmo::SCALE,
            ImGuizmo::WORLD,
            glm::value_ptr(model)
        );

        if (ImGuizmo::IsUsing())
            scene->GetComponent<TransformComponent>(selected)->SetFromMatrix(model);
    }
}

void ViewportPanel::CameraInput(Scene* scene, EntityID selected) {
    if (ImGui::IsItemHovered() && InputManager::IsMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
        if (InputManager::IsKeyDown(GLFW_KEY_LEFT_ALT) && InputManager::IsKeyDown(GLFW_KEY_LEFT_SHIFT)) {
            OnCameraPan(InputManager::GetMouseDelta());
        } else if (InputManager::IsKeyDown(GLFW_KEY_LEFT_ALT)) {
            OnCameraOrbit(InputManager::GetMouseDelta());
        }
    }

    if (ImGui::IsItemHovered()) {
        glm::vec2 scroll = InputManager::GetScrollDelta();
        if (scroll.y != 0.0f) 
            OnCameraZoom(scroll.y);
    }
    InputManager::ResetScrollDelta();

    if (InputManager::IsKeyPressed(GLFW_KEY_F) && selected != INVALID_ENTITY && scene->HasComponent<TransformComponent>(selected))
        focalPoint = scene->GetComponent<TransformComponent>(selected)->position;
}

void ViewportPanel::SelectionInput(ImVec2 availableSize) {
    if (ImGui::IsItemHovered() && InputManager::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) && !InputManager::IsKeyDown(GLFW_KEY_LEFT_ALT)) {
        ImVec2 mousePos = ImGui::GetMousePos();
        ImVec2 winPos   = ImGui::GetItemRectMin();
        ImVec2 localPos = ImVec2(mousePos.x - winPos.x, mousePos.y - winPos.y);

        int px = static_cast<int>(localPos.x);
        int py = static_cast<int>(availableSize.y - localPos.y);

        if (px >= 0 && py >= 0 && px < viewportWidth && py < viewportHeight) {
            unsigned char data[3];
            idEncodingFBO.Bind();
            glReadBuffer(GL_COLOR_ATTACHMENT0);
            glReadPixels(px, py, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, data);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            int pickedID = data[0] + (data[1] << 8) + (data[2] << 16);

            if (pickedID != 0) {
                GetEditor()->SetSelectedEntity(pickedID);
            } else {
                GetEditor()->SetSelectedEntity(INVALID_ENTITY);
            }
        }
    }
}

void ViewportPanel::OnCameraOrbit(glm::vec2 delta) {
    yaw -= delta.x * orbitSpeed;
    pitch -= delta.y * orbitSpeed;
    pitch = glm::clamp(pitch, glm::radians(-89.0f), glm::radians(89.0f));
}

void ViewportPanel::OnCameraPan(glm::vec2 delta) {
    glm::vec3 forward(
        -cos(pitch) * sin(yaw),
        sin(pitch),
        -cos(pitch) * cos(yaw)
    );
    glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
    glm::vec3 up    = glm::normalize(glm::cross(right, forward));

    focalPoint -= right * delta.x * panSpeed;
    focalPoint += up * delta.y * panSpeed;
}

void ViewportPanel::OnCameraZoom(float delta) {
    distance -= delta * zoomSpeed;
    distance = glm::clamp(distance, 1.0f, 20.0f);
}

glm::vec3 ViewportPanel::CalculateCameraPosition() {
    glm::vec3 forward(
        -cos(pitch) * sin(yaw),
        sin(pitch),
        -cos(pitch) * cos(yaw)
    );
    return focalPoint - forward * distance;
}
