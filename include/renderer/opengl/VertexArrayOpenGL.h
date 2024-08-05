#pragma once
#ifndef _RENDERER_VERTEX_ARRAY_OPENGL_H
#define _RENDERER_VERTEX_ARRAY_OPENGL_H

#include "renderer/VertexArray.h"

namespace sntl
{
	class SNTL_API VertexArrayOpenGL : public IVertexArray
	{
	public:
		VertexArrayOpenGL();
		~VertexArrayOpenGL();

		void bind() const override;
		void unbind() const override;

		void addVertexBuffer(const RefPtr<IVertexBuffer>& vertexBuffer);
		void setIndexBuffer(const RefPtr<IIndexBuffer>& indexBuffer);

		const std::vector<RefPtr<IVertexBuffer>>& getVertexBuffers() const { return vertexBuffers_; }
		const RefPtr<IIndexBuffer>& getIndexBuffer() const { return indexBuffer_; }

	private:
		uint32_t arrayID_;
		uint32_t vertexBufferIndex_;

		std::vector<RefPtr<IVertexBuffer>> vertexBuffers_;
		RefPtr<IIndexBuffer> indexBuffer_;
	};
}

#endif