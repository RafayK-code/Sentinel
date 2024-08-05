#pragma once
#ifndef _RENDERER_SHADER_H
#define _RENDERER_SHADER_H

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#include "core/Core.h"

namespace sntl
{
	class SNTL_API IShader
	{
	public:
		virtual ~IShader() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void setInt(const std::string& name, int value) = 0;
		virtual void setIntArray(const std::string& name, int* values, uint32_t count) = 0;
		
		virtual void setFloat(const std::string& name, float value) = 0;
		virtual void setFloat2(const std::string& name, const glm::vec2& value) = 0;
		virtual void setFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void setFloat4(const std::string& name, const glm::vec4& value) = 0;

		virtual void setMat3(const std::string& mame, const glm::mat3& value) = 0;
		virtual void setMat4(const std::string& name, const glm::mat4& value) = 0;

		virtual const std::string& getName() const = 0;

		static RefPtr<IShader> create(const std::string& file);
		static RefPtr<IShader> create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
	};

	class SNTL_API ShaderCache
	{
	public:
		ShaderCache();
		~ShaderCache();

		void add(const std::string& name, const RefPtr<IShader>& shader);
		void add(const RefPtr<IShader>& shader);

		RefPtr<IShader> load(const std::string& name, const std::string& file);
		RefPtr<IShader> load(const std::string& file);

		RefPtr<IShader> get(const std::string& name) const;

		bool exists(const std::string& name) const;

	private:
		std::unordered_map<std::string, RefPtr<IShader>> shaderMap_;
	};
}

#endif