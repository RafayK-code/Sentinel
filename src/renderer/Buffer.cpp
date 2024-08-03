#include "renderer/Buffer.h"

#include "core/Dbg.h"
#include "renderer/opengl/BufferOpenGL.h"

namespace sntl
{
    uint32_t shaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Bool: return 1;

        case ShaderDataType::Float: return 4;
        case ShaderDataType::Float2: return 4 * 2;
        case ShaderDataType::Float3: return 4 * 3;
        case ShaderDataType::Float4: return 4 * 4;

        case ShaderDataType::Int: return 4;
        case ShaderDataType::Int2: return 4 * 2;
        case ShaderDataType::Int3: return 4 * 3;
        case ShaderDataType::Int4: return 4 * 4;

        case ShaderDataType::Mat3: return 4 * 3 * 3;
        case ShaderDataType::Mat4: return 4 * 4 * 4;
        }

        DBG_ASSERT(false, "Unknown shader data type!");
        return 0;
    }

    uint32_t BufferElement::getComponentCount() const
    {
        switch (type)
        {
        case ShaderDataType::Bool: return 1;
        
        case ShaderDataType::Float: return 1;
        case ShaderDataType::Float2: return 2;
        case ShaderDataType::Float3: return 3;
        case ShaderDataType::Float4: return 4;

        case ShaderDataType::Int: return 1;
        case ShaderDataType::Int2: return 2;
        case ShaderDataType::Int3: return 3;
        case ShaderDataType::Int4: return 4;

        case ShaderDataType::Mat3: return 3;
        case ShaderDataType::Mat4: return 4;
        }

        DBG_ASSERT(false, "Unknown shader data type!");
        return 0;
    }

    void BufferLayout::calculateOffsetStide()
    {
        size_t offset = 0;
        stride_ = 0;

        for (auto& element : elements_)
        {
            element.offset = offset;
            offset += element.size;
            stride_ += element.size;
        }
    }

#if SNTL_RENDERER_API == SNTL_RENDERER_OPENGL

    RefPtr<IVertexBuffer> createVertexBuffer(uint32_t size)
    {
        return makeRef<VertexBufferOpenGL>(size);
    }

    RefPtr<IVertexBuffer> createVertexBuffer(float* vertices, uint32_t size)
    {
        return makeRef<VertexBufferOpenGL>(vertices, size);
    }

    RefPtr<IIndexBuffer> createIndexBuffer(uint32_t* indices, uint32_t count)
    {
        return makeRef<IndexBufferOpenGL>(indices, count);
    }

#else

    RefPtr<IVertexBuffer> createVertexBuffer(uint32_t size)
    {
        return nullptr;
    }

    RefPtr<IVertexBuffer> createVertexBuffer(float* vertices, uint32_t size)
    {
        return nullptr;
    }

    RefPtr<IIndexBuffer> createIndexBuffer(uint32_t* indices, uint32_t count)
    {
        return nullptr;
    }

#endif
}