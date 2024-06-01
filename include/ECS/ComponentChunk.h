#pragma once
#ifndef _COMPONENT_CHUNK
#define _COMPONENT_CHUNK

#include "Core.h"
#include "Types.h"

namespace sntl
{
    class ENGINE_API ComponentChunk
    {
    public:
        ComponentChunk(size_t buffSize);
        ~ComponentChunk();

        ComponentChunk(const ComponentChunk& other);
        ComponentChunk& operator=(const ComponentChunk& other);

        ComponentChunk(ComponentChunk&& other) noexcept;
        ComponentChunk& operator=(ComponentChunk&& other) noexcept;

        void* get();

    private:
        uint8_t* bytes_;
        size_t buffSize_;
    };
}

#endif