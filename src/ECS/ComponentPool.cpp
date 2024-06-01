#include "ECS/ComponentPool.h"

namespace sntl
{
    ComponentPool::ComponentPool(size_t elementSize, size_t numElements)
    {
        for (size_t i = 0; i < numElements; i++)
            chunks_.emplace_back(elementSize);
    }

    ComponentPool::~ComponentPool()
    {
    }

    void* ComponentPool::get(size_t index)
    {
        return chunks_[index].get();
    }
}