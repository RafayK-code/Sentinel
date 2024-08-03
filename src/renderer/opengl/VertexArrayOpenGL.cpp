#include "renderer/opengl/VertexArrayOpenGL.h"

#include <glad/glad.h>

#include "core/Dbg.h"

namespace sntl
{
	static GLenum ShaderDataTypeToGL(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Bool: return GL_BOOL;

		case ShaderDataType::Float: return GL_FLOAT;
		case ShaderDataType::Float2: return GL_FLOAT;
		case ShaderDataType::Float3: return GL_FLOAT;
		case ShaderDataType::Float4: return GL_FLOAT;

		case ShaderDataType::Int: return GL_INT;
		case ShaderDataType::Int2: return GL_INT;
		case ShaderDataType::Int3: return GL_INT;
		case ShaderDataType::Int4: return GL_INT;

		case ShaderDataType::Mat3: return GL_FLOAT;
		case ShaderDataType::Mat4: return GL_FLOAT;
		}

		DBG_ASSERT(false, "Unknown shader data type!");
		return GL_NONE;
	}

	VertexArrayOpenGL::VertexArrayOpenGL()
		: vertexBufferIndex_(0)
	{
		glCreateVertexArrays(1, &arrayID_);
	}

	VertexArrayOpenGL::~VertexArrayOpenGL()
	{
		glDeleteVertexArrays(1, &arrayID_);
	}

	void VertexArrayOpenGL::bind() const
	{
		glBindVertexArray(arrayID_);
	}

	void VertexArrayOpenGL::unbind() const
	{
		glBindVertexArray(0);
	}

	void VertexArrayOpenGL::addVertexBuffer(const RefPtr<IVertexBuffer>& vertexBuffer)
	{
		DBG_ASSERT(vertexBuffer->getLayout().getElements().size() > 0, "Vertex buffer has no layout");

		bind();
		vertexBuffer->bind();

		const BufferLayout& layout = vertexBuffer->getLayout();
		for (const auto& element : layout)
		{
			switch (element.type)
			{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			{
				glEnableVertexAttribArray(vertexBufferIndex_);
				glVertexAttribPointer(
					vertexBufferIndex_,
					element.getComponentCount(),
					ShaderDataTypeToGL(element.type),
					element.normalized ? GL_TRUE : GL_FALSE,
					layout.getStride(),
					(const void*)element.offset
				);
				vertexBufferIndex_++;
				break;
			}

			case ShaderDataType::Bool:
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			{
				glEnableVertexAttribArray(vertexBufferIndex_);
				glVertexAttribIPointer(
					vertexBufferIndex_,
					element.getComponentCount(),
					ShaderDataTypeToGL(element.type),
					layout.getStride(),
					(const void*)element.offset
				);
				vertexBufferIndex_++;
				break;
			}

			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
			{
				uint8_t count = (uint8_t)element.getComponentCount();
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(vertexBufferIndex_);
					glVertexAttribPointer(
						vertexBufferIndex_,
						count,
						ShaderDataTypeToGL(element.type),
						element.normalized ? GL_TRUE : GL_FALSE,
						layout.getStride(),
						(const void*)(element.offset + sizeof(float) * count * i)
					);
					glVertexAttribDivisor(vertexBufferIndex_, 1);
					vertexBufferIndex_++;
				}
				break;
			}
			default:
				DBG_ASSERT(false, "Unknown shader data type!");
				break;
			}
		}

		vertexBuffers_.push_back(vertexBuffer);
	}

	void VertexArrayOpenGL::setIndexBuffer(const RefPtr<IIndexBuffer>& indexBuffer)
	{
		bind();
		indexBuffer->bind();

		indexBuffer_ = indexBuffer;
	}
}