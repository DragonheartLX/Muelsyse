#pragma once
#include "Muelsyse/Core/Core.h"

#include <string>
#include <glm/glm.hpp>

namespace mul 
{
	class MUL_API Shader
	{
	public:
		virtual ~Shader() = default;


		virtual void bind() const = 0;
		virtual void unBind() const = 0;

		static Shader* create(const std::string& vertexSrc, const std::string& fragmentSrc, bool isFromFile = true);
	private:
		uint32_t m_RendererID;
	};
}