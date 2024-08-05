#include "renderer/Shader.h"

#include "core/Dbg.h"
#include "renderer/opengl/ShaderOpenGL.h"
#include "renderer/RendererPlatform.h"

namespace sntl
{
	RefPtr<IShader> IShader::create(const std::string& file)
	{
		switch (RendererPlatform::ref().getPlatform())
		{
		case RendererPlatform::Platform::OpenGL: return makeRef<ShaderOpenGL>(file);
		}

		DBG_ASSERT(false, "Unrecognized renderer platform specified");
		return nullptr;
	}

	RefPtr<IShader> IShader::create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (RendererPlatform::ref().getPlatform())
		{
		case RendererPlatform::Platform::OpenGL: return makeRef<ShaderOpenGL>(name, vertexSrc, fragmentSrc);
		}

		DBG_ASSERT(false, "Unrecognized renderer platform specified");
		return nullptr;
	}

	ShaderCache::ShaderCache()
	{
	}

	ShaderCache::~ShaderCache()
	{
	}

	void ShaderCache::add(const std::string& name, const RefPtr<IShader>& shader)
	{
		if (exists(name))
			DBG_WARN("Shader already exists. Overwriting");

		shaderMap_[name] = shader;
	}

	void ShaderCache::add(const RefPtr<IShader>& shader)
	{
		add(shader->getName(), shader);
	}

	RefPtr<IShader> ShaderCache::load(const std::string& name, const std::string& file)
	{
		RefPtr<IShader> shader = IShader::create(file);
		add(name, shader);
		return shader;
	}

	RefPtr<IShader> ShaderCache::load(const std::string& file)
	{
		RefPtr<IShader> shader = IShader::create(file);
		add(shader);
		return shader;
	}

	RefPtr<IShader> ShaderCache::get(const std::string& name) const
	{
		if (!exists(name))
		{
			DBG_WARN("Shader does not exist");
			return nullptr;
		}

		return shaderMap_.find(name)->second;
	}

	bool ShaderCache::exists(const std::string& name) const
	{
		return shaderMap_.find(name) != shaderMap_.end();
	}
}