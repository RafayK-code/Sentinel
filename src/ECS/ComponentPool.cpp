#include "ECS/ComponentPool.h"

#include <unordered_map>

namespace sntl
{
    ComponentPool::ComponentPool(size_t elementSize, size_t numElements)
        : sparseSet_(numElements, size_t(-1)), chunkSize_(elementSize), chunksEnd_(0)
    {
    }

    ComponentPool::~ComponentPool()
    {
    }

    void* ComponentPool::getChunk(size_t index)
    {
        if (sparseSet_[index] == size_t(-1))
            addIndex(index);

        return chunks_[sparseSet_[index]].get();
    }

    void ComponentPool::freeChunk(size_t index)
    {
        denseSet_[sparseSet_[index]] = denseSet_[chunksEnd_ - 1];
        sparseSet_[denseSet_[chunksEnd_ - 1]] = sparseSet_[index];
        sparseSet_[index] = -1;
        chunksEnd_--;
    }

    void ComponentPool::addIndex(size_t index)
    {
        if (chunksEnd_ < chunks_.size())
        {
            sparseSet_[index] = chunksEnd_;
            denseSet_[chunksEnd_] = index;
        }

        else
        {
            size_t newIndex = chunks_.size();
            sparseSet_[index] = newIndex;
            denseSet_.push_back(index);
            chunks_.push_back(ComponentChunk(chunkSize_));
        }

        chunksEnd_++;
    }
}