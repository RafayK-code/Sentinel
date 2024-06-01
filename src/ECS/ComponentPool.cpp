#include "ECS/ComponentPool.h"

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

        return chunks_[sparseSet_[index]].chunk.get();
    }

    void ComponentPool::freeChunk(size_t index)
    {
        memset(chunks_[sparseSet_[index]].chunk.get(), 0, chunkSize_);
        chunks_[sparseSet_[index]].sparseIndex = size_t(-1);
        auto logicalEnd = std::remove_if(chunks_.begin(), chunks_.end(), [](DenseChunk& chnk) {
            return chnk.sparseIndex = size_t(-1);
        });
        chunksEnd_ = logicalEnd - chunks_.begin();
    }

    void ComponentPool::addIndex(size_t index)
    {
        if (chunksEnd_ < chunks_.size())
        {
            sparseSet_[index] = chunksEnd_;
            chunks_[chunksEnd_].sparseIndex = index;
        }

        else
        {
            size_t newIndex = chunks_.size();
            sparseSet_[index] = newIndex;
            chunks_.push_back({ ComponentChunk(chunkSize_), index });
        }

        chunksEnd_++;
    }

    ComponentPool::Iterator ComponentPool::begin()
    {
        return Iterator(chunks_, 0);
    }

    ComponentPool::Iterator ComponentPool::end()
    {
        return Iterator(chunks_, chunksEnd_);
    }
}