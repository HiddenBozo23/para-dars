#pragma once

#include "para-dars/ecs/EntityManager.h"
#include "para-dars/ecs/Serialisable.h"

struct HierarchyComponent : Serialisable{
    EntityID parent = INVALID_ENTITY;
    std::vector<EntityID> children;

    std::string Serialise() override {
        nlohmann::json j;
        j["parent"] = parent;
        j["children"] = children;
        return j.dump();
    }

    void Deserialise(const std::string& data) override {
        nlohmann::json j = nlohmann::json::parse(data);
        parent = j.value("parent", INVALID_ENTITY);
        children = j.value("children", std::vector<EntityID>{});
    }
};