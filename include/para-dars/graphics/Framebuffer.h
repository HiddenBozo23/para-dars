#pragma once
#include <glad/glad.h>

class Framebuffer {
    public:
        Framebuffer() = default;
        ~Framebuffer() { Destroy(); }

        void Create(int width, int height);
        void Destroy();

        void Resize(int newWidth, int newHeight);
        void Bind();
        void Unbind();
        
        unsigned int GetColorAttachment();
    
    private:
        unsigned int fbo = 0;
        unsigned int colorAttachment = 0;
        unsigned int depthAttachment = 0;
        int width = 0, height = 0;
};