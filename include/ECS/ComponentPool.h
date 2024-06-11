#pragma once
#ifndef _COMPONENT_POOL_H
#define _COMPONENT_POOL_H

#include <vector>

#include "Types.h"
#include "ComponentChunk.h"

namespace sntl
{
    class ENGINE_API ComponentPool
    {
    public:
        ComponentPool(size_t elementSize, size_t numElements);
        ~ComponentPool();

        void freeChunk(internal::EntityIndex index);
        void* getChunk(internal::EntityIndex index);

        const std::vector<internal::EntityIndex>& getDenseChunks() { return denseSet_; }

        size_t numChunks() { return chunksEnd_;  }

    private:

        void addIndex(internal::EntityIndex index);

        std::vector<ComponentChunk> chunks_;
        std::vector<internal::EntityIndex> denseSet_;
        std::vector<internal::EntityIndex> sparseSet_;
        size_t chunkSize_;
        size_t chunksEnd_;
    };
}

#endif