#pragma once

#include "para-dars/ecs/EntityManager.h"

struct HierarchyComponent {
    EntityID parent = INVALID_ENTITY;
    std::vector<EntityID> children;
};