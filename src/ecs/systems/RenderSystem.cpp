#include "para-dars/ecs/systems/RenderSystem.h"
#include "para-dars/ecs/components/ModelComponent.h"
#include "para-dars/ecs/components/RenderComponent.h"
#include "para-dars/core/LogManager.h"

RenderSystem::RenderSystem(Scene* scene, int viewportWidth, int viewportHeight)
    : System(scene), viewportWidth(viewportWidth), viewportHeight(viewportHeight) {
    requiredComponents = {
        typeid(TransformComponent),
        typeid(ModelComponent),
        typeid(RenderComponent)
    };
    framebuffer.Create(viewportWidth, viewportHeight);
}

void RenderSystem::Render(Camera& camera) {
    // placeholder dummy render system
    framebuffer.Bind();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = glm::inverse(camera.transform.GetMatrix());
    glm::mat4 projection = camera.camera.GetProjectionMatrix(static_cast<float>(viewportWidth) / viewportHeight);

    for (EntityID entity : trackedEntities) {
        RenderComponent* meshRender = GetScene()->GetComponent<RenderComponent>(entity);
        if (!meshRender->visible) continue;
        ModelComponent* meshModel = GetScene()->GetComponent<ModelComponent>(entity);
        if (!meshModel->model) continue;
        TransformComponent* meshTrans = GetScene()->GetComponent<TransformComponent>(entity);
        
        Shader& shader = Shader("shaders/model.vert", "shaders/model.frag");
        shader.Use();

        shader.SetInt("numDirectionalLights", 1);
        shader.SetInt("numPointLights", 0);
        shader.SetInt("numSpotLights", 0);

        shader.SetVec3("directionalLights[0].direction", glm::vec3(-0.2f, -1.0f, -0.3f));
        shader.SetVec3("directionalLights[0].ambient", glm::vec3(0.9f, 0.9f, 0.9f));\
        shader.SetVec3("directionalLights[0].diffuse", glm::vec3(1.0f, 0.95f, 0.8f));
        shader.SetVec3("directionalLights[0].specular", glm::vec3(1.0f, 0.95f, 0.8f));

        glm::mat4 model = meshTrans->GetMatrix();
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));

        shader.SetMat4("model", model);
        shader.SetMat4("view", view);
        shader.SetMat4("projection", projection);
        shader.SetMat3("normalMatrix", normalMatrix);

        meshModel->model->Draw(shader);

        glUseProgram(0);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int RenderSystem::GetFramebufferTexture() {
    return framebuffer.GetColorAttachment();
}

void RenderSystem::ResizeFrameBuffer(int width, int height) {
    framebuffer.Resize(width, height);

    viewportWidth = width;
    viewportHeight = height;
}
