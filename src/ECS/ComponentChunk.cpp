#include "ECS/ComponentChunk.h"

namespace sntl
{
    ComponentChunk::ComponentChunk(size_t buffSize)
        : buffSize_(buffSize), bytes_(new uint8_t[buffSize])
    {
    }

    ComponentChunk::~ComponentChunk()
    {
        if (bytes_)
            delete[] bytes_;
    }

    ComponentChunk::ComponentChunk(const ComponentChunk& other)
        : buffSize_(other.buffSize_), bytes_(new uint8_t[other.buffSize_])
    {
        std::memcpy(bytes_, other.bytes_, buffSize_);
    }

    ComponentChunk& ComponentChunk::operator=(const ComponentChunk& other)
    {
        if (bytes_)
            delete[] bytes_;

        buffSize_ = other.buffSize_;
        bytes_ = new uint8_t[buffSize_];

        std::memcpy(bytes_, other.bytes_, buffSize_);

        return *this;
    }

    ComponentChunk::ComponentChunk(ComponentChunk&& other) noexcept
        : buffSize_(other.buffSize_), bytes_(other.bytes_)
    {
        other.bytes_ = nullptr;
    }

    ComponentChunk& ComponentChunk::operator=(ComponentChunk&& other) noexcept
    {
        if (bytes_)
            delete[] bytes_;

        buffSize_ = other.buffSize_;
        bytes_ = other.bytes_;

        other.bytes_ = nullptr;

        return *this;
    }

    void* ComponentChunk::get()
    {
        return (void*)bytes_;
    }
}