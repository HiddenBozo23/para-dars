#pragma once

#include <string>
#include <functional>
#include <unordered_map>

#include "para-dars/editor/Panel.h"
#include "para-dars/ecs/Scene.h"

class InspectorPanel : public Panel {
    public:
        InspectorPanel(Editor* editor, Scene* scene);

        void OnImGuiRender() override;

        template<typename T>
        void RegisterDraw(const std::string& name, std::function<void(T&)> drawer);

    private:
        Scene* scene;

        struct DrawData {
            std::string name;
            std::function<bool(EntityID)> checkFunc;
            std::function<void(EntityID)> drawFunc;
        };
        std::unordered_map<std::type_index, DrawData> componentDrawers;
};

template<typename T>
void InspectorPanel::RegisterDraw(const std::string& name, std::function<void(T&)> drawer) {
    DrawData data;
    data.name = name;
    data.checkFunc = [this](EntityID id) { return scene->HasComponent<T>(id); };
    data.drawFunc = [this, drawer](EntityID id) {
        T* ptr = scene->GetComponent<T>(id);
        if (!ptr) return;
        T& comp = *ptr;
        drawer(comp);
    };
    componentDrawers[std::type_index(typeid(T))] = data;
}