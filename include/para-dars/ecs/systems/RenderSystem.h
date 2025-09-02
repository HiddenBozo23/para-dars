#pragma once

#include "para-dars/ecs/System.h"
#include "para-dars/ecs/Scene.h"
#include "para-dars/ecs/components/CameraComponent.h"
#include "para-dars/ecs/components/TransformComponent.h"
#include "para-dars/graphics/FrameBuffer.h"

class RenderSystem : public System {
    public:
        explicit RenderSystem(Scene* scene, int viewportWidth, int viewportHeight);
        void Update(float detaTime) override {}

        void Render(Camera& camera);
        unsigned int GetFramebufferTexture();
        void ResizeFrameBuffer(int width, int height);

    private:
        Framebuffer framebuffer;

        int viewportWidth, viewportHeight;
};
