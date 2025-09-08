#pragma once

#include <string>

#include "para-dars/ecs/Serialisable.h"

struct NameComponent : Serialisable {
    std::string name;

    std::string Serialise() override {
        nlohmann::json j;
        j["name"] = name;
        return j.dump();
    }

    void Deserialise(const std::string& data) override {
        nlohmann::json j = nlohmann::json::parse(data);
        name = j.value("name", "");
    }
};