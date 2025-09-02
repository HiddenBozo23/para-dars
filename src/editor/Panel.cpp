#include "para-dars/editor/Panel.h"
#include "para-dars/core/LogManager.h"

Panel::Panel(Editor* editor)
    : editor(editor) {
    backupEditor = std::make_unique<Editor>();
}

Editor* Panel::GetEditor() {
    if (editor) return editor;

    LogManager::Log(LogType::Error, "ERROR: PANEL POINTER TO EDITOR IS NULL, USING DEFAULT EDITOR");
    return backupEditor.get();
}
