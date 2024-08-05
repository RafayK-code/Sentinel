#include "renderer/VertexArray.h"

#include "core/Dbg.h"
#include "renderer/opengl/VertexArrayOpenGL.h"
#include "renderer/RendererPlatform.h"

namespace sntl
{
	RefPtr<IVertexArray> IVertexArray::create()
	{
        switch (RendererPlatform::ref().getPlatform())
        {
        case RendererPlatform::Platform::OpenGL: return makeRef<VertexArrayOpenGL>();
        }

        DBG_ASSERT(false, "Unrecognized renderer platform specified");
        return nullptr;
	}
}