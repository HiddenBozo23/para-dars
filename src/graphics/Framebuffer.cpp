#include "para-dars/graphics/FrameBuffer.h"
#include "para-dars/core/LogManager.h"

void Framebuffer::Create(int width, int height) {
    if (width <= 0 || height <= 0) {
        LogManager::Log(LogType::Warning, "WARNING: SKIPED FRAMEBUFFER CREATION FOR INVALID SIZE: (" + std::to_string(width) + " x " + std::to_string(height) + ")");
        return;
    }

    this->width = width;
    this->height = height;
    
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    // color attachment
    glGenTextures(1, &colorAttachment);
    glBindTexture(GL_TEXTURE_2D, colorAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment, 0);
    // depth attachment
    glGenRenderbuffers(1, &depthAttachment);
    glBindRenderbuffer(GL_RENDERBUFFER, depthAttachment);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthAttachment);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        LogManager::Log(LogType::Error, "ERROR: FRAMEBUFFER IS NOT COMPLETE");
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Destroy() {
    if (colorAttachment) glDeleteTextures(1, &colorAttachment);
    if (depthAttachment) glDeleteRenderbuffers(1, &depthAttachment);
    if (fbo) glDeleteFramebuffers(1, &fbo);
    fbo = colorAttachment = depthAttachment = 0;
}

void Framebuffer::Resize(int newWidth, int newHeight) {
    if (newWidth == width && newHeight == height) return;
    Destroy();
    Create(newWidth, newHeight);
}

void Framebuffer::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, width, height);
}

void Framebuffer::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int Framebuffer::GetColorAttachment() {
    return colorAttachment;
}  