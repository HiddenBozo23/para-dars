#pragma once

#include "para-dars/editor/Panel.h"

class LogPanel : public Panel {
    public:
        LogPanel(Editor* editor);

        void OnImGuiRender() override;
};