#pragma once
#ifndef _TYPES_H
#define _TYPES_H

#include <bitset>

namespace sntl
{
    using EntityID = uint64_t;
    using ComponentID = uint8_t;

    constexpr ComponentID MAX_COMPONENTS = 32;
    constexpr size_t ENGINE_MAX_ENTITIES = 100000;

    using ComponentSignature = std::bitset<MAX_COMPONENTS>;

    inline ComponentID getNewComponentID()
    {
        static ComponentID newCID = 0;
        return newCID++;
    }

    template<typename T>
    ComponentID getComponentID()
    {
        static ComponentID cID = getNewComponentID();
        return cID;
    }

    namespace internal
    {
        using EntityIndex = uint32_t;
        using EntityVersion = uint32_t;
    }
}

#endif