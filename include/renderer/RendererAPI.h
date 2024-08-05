#pragma once
#ifndef _RENDERER_API
#define _RENDERER_API

#include <glm/glm.hpp>

#include "VertexArray.h"
#include "core/Core.h"

namespace sntl
{
	class IRendererAPI
	{
	public:
		virtual ~IRendererAPI() = default;

		virtual void init() = 0;
		virtual void setViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) = 0;
		virtual void setClearColor(const glm::vec4& color) = 0;
		virtual void clear() = 0;

		virtual void drawIndexed() = 0;
		virtual void drawLines() = 0;

		virtual void setLineWidth(float width) = 0;

		static ScopePtr<IRendererAPI> create();
	};
}

#endif