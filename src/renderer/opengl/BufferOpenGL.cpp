#include "renderer/opengl/BufferOpenGL.h"

#include <glad/glad.h>

namespace sntl
{
    VertexBufferOpenGL::VertexBufferOpenGL(uint32_t size)
    {
        glCreateBuffers(1, &bufferID_);
        glBindBuffer(GL_ARRAY_BUFFER, bufferID_);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    VertexBufferOpenGL::VertexBufferOpenGL(float* vertices, uint32_t size)
    {
        glCreateBuffers(1, &bufferID_);
        glBindBuffer(GL_ARRAY_BUFFER, bufferID_);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    VertexBufferOpenGL::~VertexBufferOpenGL()
    {
        glDeleteBuffers(1, &bufferID_);
    }

    void VertexBufferOpenGL::bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, bufferID_);
    }

    void VertexBufferOpenGL::unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBufferOpenGL::setData(void* data, uint32_t size)
    {
        glBindBuffer(GL_ARRAY_BUFFER, bufferID_);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }
}