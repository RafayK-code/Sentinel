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

        struct DenseChunk
        {
            ComponentChunk chunk;
            size_t sparseIndex;
        };

        class ChunkIterator
        {
        public:
            using valueType = size_t;
            using pointer = const size_t*;
            using reference = const size_t&;

            ChunkIterator(const std::vector<DenseChunk>& chunks, size_t index)
                : chunks_(chunks), index_(index)
            {
            }

            reference operator*() const
            {
                return chunks_[index_].sparseIndex;
            }

            ChunkIterator& operator++()
            {
                index_++;
                return *this;
            }

            ChunkIterator operator++(int)
            {
                ChunkIterator tmp = *this;
                index_++;
                return tmp;
            }

            bool operator==(const ChunkIterator& other) const
            {
                return index_ == other.index_;
            }

            bool operator!=(const ChunkIterator& other) const
            {
                return index_ != other.index_;
            }

        private:
            const std::vector<DenseChunk>& chunks_;
            size_t index_;
        };

        using Iterator = ChunkIterator;
        Iterator begin();
        Iterator end();

        size_t numChunks() { return chunksEnd_;  }

    private:

        void addIndex(size_t index);

        std::vector<DenseChunk> chunks_;
        std::vector<size_t> sparseSet_;
        size_t chunkSize_;
        size_t chunksEnd_;
    };
}

#endif