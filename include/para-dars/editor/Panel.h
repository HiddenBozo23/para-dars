#pragma once

#include <memory>

#include "para-dars/ecs/EntityManager.h"

class Editor;  // break cyclic dependancy by forward declaration

class Panel {
    public:
        explicit Panel(Editor* editor);
        virtual ~Panel() = default;

        virtual void OnImGuiRender() = 0;

    protected:
        Editor* GetEditor();

    private:
        Editor* editor;
};

#include "para-dars/editor/Editor.h"