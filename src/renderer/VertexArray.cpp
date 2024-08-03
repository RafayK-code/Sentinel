#include "renderer/VertexArray.h"

namespace sntl
{
#if SNTL_RENDERER_API == SNTL_RENDERER_OPENGL

	RefPtr<IVertexArray> createVertexArray()
	{
		return nullptr;
	}

#else 

	RefPtr<IVertexArray> createVertexArray()
	{
		return nullptr;
	}

#endif
}