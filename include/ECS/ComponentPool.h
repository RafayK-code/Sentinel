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

        void freeChunk(size_t index);
        void* getChunk(size_t index);

        const std::vector<size_t>& getDenseChunks() { return denseSet_; }

        size_t numChunks() { return chunksEnd_;  }

    private:

        void addIndex(size_t index);

        std::vector<ComponentChunk> chunks_;
        std::vector<size_t> denseSet_;
        std::vector<size_t> sparseSet_;
        size_t chunkSize_;
        size_t chunksEnd_;
    };
}

#endif