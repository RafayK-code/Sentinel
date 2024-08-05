#include "renderer/RendererPlatform.h"

namespace sntl
{
	RendererPlatform::RendererPlatform()
		: platform_(Platform::None)
	{
	}

	RendererPlatform::~RendererPlatform()
	{
	}

	void RendererPlatform::init(Platform platform)
	{
		RendererPlatform::createSingleton();
		RendererPlatform::ref().setPlatform(platform);
	}

	void RendererPlatform::destroy()
	{
		RendererPlatform::destroySingleton();
	}
}