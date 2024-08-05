#pragma once
#ifndef _RENDERER_VERTEX_ARRAY_H
#define _RENDERER_VERTEX_ARRAY_H

#include "Buffer.h"

namespace sntl
{
	class SNTL_API IVertexArray
	{
	public:
		virtual ~IVertexArray() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void addVertexBuffer(const RefPtr<IVertexBuffer>& vertexBuffer) = 0;
		virtual void setIndexBuffer(const RefPtr<IIndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<RefPtr<IVertexBuffer>>& getVertexBuffers() const = 0;
		virtual const RefPtr<IIndexBuffer>& getIndexBuffer() const = 0;

		static RefPtr<IVertexArray> create();
	};
}

#endif