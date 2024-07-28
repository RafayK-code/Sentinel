#pragma once
#ifndef _RENDERER_BUFFER_H
#define _RENDERER_BUFFER_H

#include <string>
#include <vector>

#include "core/Core.h"

namespace sntl
{
    enum class ShaderDataType
    {
        None = 0,
        Bool,
        Float, Float2, Float3, Float4,
        Int, Int2, Int3, Int4,
        Mat3, Mat4,
    };

    uint32_t SNTL_API shaderDataTypeSize(ShaderDataType type);

    class SNTL_API BufferElement
    {
    public:
        std::string name;
        ShaderDataType type;
        uint32_t size;
        size_t offset;
        bool normalized;

        BufferElement() = default;

        BufferElement(const std::string& name, ShaderDataType type, bool normalized = false)
            : name(name), type(type), size(shaderDataTypeSize(type)), offset(0), normalized(normalized)
        {
        }

        uint32_t getComponentCount() const;
    };

    class SNTL_API BufferLayout
    {
    public:
        BufferLayout()
        {
        }

        BufferLayout(std::initializer_list<BufferElement> elements)
            : elements_(elements)
        {
            calculateOffsetStide();
        }

        uint32_t getStride() const { return stride_; }
        const std::vector<BufferElement>& getElements() const { return elements_; }

        std::vector<BufferElement>::iterator begin() { return elements_.begin(); }
        std::vector<BufferElement>::iterator end() { return elements_.end(); }
        std::vector<BufferElement>::const_iterator begin() const { return elements_.begin(); }
        std::vector<BufferElement>::const_iterator end() const { return elements_.end(); }

    private:
        void calculateOffsetStide();

    private:
        std::vector<BufferElement> elements_;
        uint32_t stride_ = 0;
    };

    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void setData(void* data, uint32_t size) = 0;
        virtual void setLayout(const BufferLayout& layout) = 0;
    };

    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual uint32_t getCount() const = 0;
    };

    RefPtr<VertexBuffer> createVertexBuffer(uint32_t size);
    RefPtr<VertexBuffer> createVertexBuffer(float* vertices, uint32_t size);

    RefPtr<IndexBuffer> createIndexBuffer(uint32_t indices, uint32_t count);
}

#endif