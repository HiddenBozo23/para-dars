#include "para-dars/editor/Panel.h"
#include "para-dars/core/LogManager.h"

Panel::Panel(Editor* editor)
    : editor(editor) {}

Editor* Panel::GetEditor() {
    if (editor) return editor;

    LogManager::Log(LogType::Error, "ERROR: PANEL POINTER TO EDITOR IS NULL");
    return nullptr;
}
