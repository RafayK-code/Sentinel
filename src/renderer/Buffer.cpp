#include "renderer/Buffer.h"

#include "core/Dbg.h"
#include "renderer/opengl/BufferOpenGL.h"
#include "renderer/RendererPlatform.h"

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

    RefPtr<IVertexBuffer> IVertexBuffer::create(uint32_t size)
    {
        switch (RendererPlatform::ref().getPlatform())
        {
        case RendererPlatform::Platform::OpenGL: return makeRef<VertexBufferOpenGL>(size);
        }

        DBG_ASSERT(false, "Unrecognized renderer platform specified");
        return nullptr;
    }

    RefPtr<IVertexBuffer> IVertexBuffer::create(float* vertices, uint32_t size)
    {
        switch (RendererPlatform::ref().getPlatform())
        {
        case RendererPlatform::Platform::OpenGL: return makeRef<VertexBufferOpenGL>(vertices, size);
        }

        DBG_ASSERT(false, "Unrecognized renderer platform specified");
        return nullptr;
    }

    RefPtr<IIndexBuffer> IIndexBuffer::create(uint32_t* indices, uint32_t count)
    {
        switch (RendererPlatform::ref().getPlatform())
        {
        case RendererPlatform::Platform::OpenGL: return makeRef<IndexBufferOpenGL>(indices, count);
        }

        DBG_ASSERT(false, "Unrecognized renderer platform specified");
        return nullptr;
    }
}