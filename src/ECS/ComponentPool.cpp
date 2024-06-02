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
        denseSet_[sparseSet_[index]] = size_t(-1);
        std::unordered_map<size_t, size_t> indexMap;

        std::remove_if(chunks_.begin(), chunks_.end(), [&](const ComponentChunk& chunk) {
            auto index = &chunk - &chunks_[0];

            if (denseSet_[index] == size_t(-1))
                return true;

            indexMap[index] = indexMap.size();
            return false;
        });

        auto logicalEnd = std::remove_if(denseSet_.begin(), denseSet_.end(), [&](size_t denseIndex) {
            if (denseIndex == size_t(-1))
                return true;

            sparseSet_[denseIndex] = indexMap[denseIndex];
            return false;
        });

        chunksEnd_ = logicalEnd - denseSet_.begin();
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