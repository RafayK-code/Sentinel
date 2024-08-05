#pragma once
#ifndef _RENDERER_SHADER_OPENGL_H
#define _RENDERER_SHADER_OPENGL_H

#include <vector>

#include "renderer/Shader.h"

typedef unsigned int GLenum;

namespace sntl
{
	class SNTL_API ShaderOpenGL : public IShader
	{
	public:
		ShaderOpenGL(const std::string& path);
		ShaderOpenGL(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		
		~ShaderOpenGL();

		void bind() const override;
		void unbind() const override;

		void setInt(const std::string& name, int value) override;
		void setIntArray(const std::string& name, int* values, uint32_t count) override;

		void setFloat(const std::string& name, float value) override;
		void setFloat2(const std::string& name, const glm::vec2& value) override;
		void setFloat3(const std::string& name, const glm::vec3& value) override;
		void setFloat4(const std::string& name, const glm::vec4& value) override;

		void setMat3(const std::string& mame, const glm::mat3& value) override;
		void setMat4(const std::string& name, const glm::mat4& value) override;

		const std::string& getName() const override { return name_; }

		void uploadUniformInt(const std::string& name, int value);
		void uploadUniformIntArray(const std::string& name, int* value, uint32_t count);

		void uploadUniformFloat(const std::string& name, float value);
		void uploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void uploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void uploadUniformFloat4(const std::string& name, const glm::vec4& value);

		void uploadUniformMat3(const std::string& name, const glm::mat3& value);
		void uploadUniformMat4(const std::string& name, const glm::mat4& value);

	private:
		std::string& readFile(const std::string& file);
		std::unordered_map<GLenum, std::string> preProcess(const std::string& source);

		void compileOrGetOpenGLBinaries();
		void createProgram();
		void reflect(GLenum stage, const std::vector<uint32_t>& shaderData);

	private:
		uint32_t shaderID_;

		std::string filePath_;
		std::string name_;

		std::unordered_map<GLenum, std::vector<uint32_t>> openGLSpirv_;
		std::unordered_map<GLenum, std::string> openGLSourceCode_;
	};
}

#endif