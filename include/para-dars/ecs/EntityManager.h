#pragma once

#include <cstdint>
#include <vector>

// EntityID is just a type alias for a fixed size int.
using EntityID = uint32_t;
const EntityID INVALID_ENTITY = 0;

class EntityManager {
    public:
        static EntityID CreateEntity();
        static void DestroyEntity(EntityID id);
        static bool isValid(EntityID id);

    private:
        static EntityID nextID;
        static std::vector<bool> alive;
        static std::vector<EntityID> freeIDs;
};