#pragma once
#ifndef _COMPONENT_POOL_H
#define _COMPONENT_POOL_H

#include <vector>

#include "ComponentChunk.h"

namespace sntl
{
    class ENGINE_API ComponentPool
    {
    public:
        ComponentPool(size_t elementSize, size_t numElements);
        ~ComponentPool();

        void* get(size_t index);

    private:
        std::vector<ComponentChunk> chunks_;
    };
}

#endif