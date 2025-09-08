#pragma once

#include <string>
#include <nlohmann/json.hpp>

struct Serialisable {
    public:
        virtual ~Serialisable() = default;

        virtual std::string Serialise() = 0;
        virtual void Deserialise(const std::string& data) = 0;
};