#pragma once
#ifndef _RENDERER_PLATFORM_H
#define _RENDERER_PLATFORM_H

#include "core/Singleton.h"
#include "core/Core.h"

namespace sntl
{
	class SNTL_API RendererPlatform : public Singleton<RendererPlatform>
	{
		friend class Singleton<RendererPlatform>;
	public:
		~RendererPlatform();

		enum class Platform
		{
			None,
			OpenGL,
		};

		Platform getPlatform() const { return platform_; }
		void setPlatform(Platform platform) { platform_ = platform; }

		static void init(Platform platform = Platform::None);
		static void destroy();

	private:
		RendererPlatform();

		Platform platform_;
	};
}

#endif