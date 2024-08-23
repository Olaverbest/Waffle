#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Waffle {

	class Shader
	{
	private:
		uint32_t m_RendererID;
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Shader* Create(const std::string& filepath);
		static Shader* Create(const std::string& vertexSource, const std::string& fragmentSource);
	};
}