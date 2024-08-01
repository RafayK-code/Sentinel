#pragma once
#ifndef _RENDERER_BUFFER_OPENGL_H
#define _RENDERER_BUFFER_OPENGL_H

#include "renderer/Buffer.h"

namespace sntl
{
    class VertexBufferOpenGL : public VertexBuffer
    {
    public:
        VertexBufferOpenGL(uint32_t size);
        VertexBufferOpenGL(float* vertices, uint32_t size);

        ~VertexBufferOpenGL();

        void bind() const override;
        void unbind() const override;

        void setData(void* data, uint32_t size) override;

        const BufferLayout& getLayout() const override { return layout_; }
        void setLayout(const BufferLayout& layout) override { layout_ = layout; }

    private:
        uint32_t bufferID_;
        BufferLayout layout_;
    };

    class IndexBufferOpenGL : public IndexBuffer
    {

    };
}

#endif