#pragma once

class FullscreenQuad {
    public:
        FullscreenQuad();
        ~FullscreenQuad();

        void Draw();
    private:
        unsigned int vao = 0;
        unsigned int vbo = 0;
};