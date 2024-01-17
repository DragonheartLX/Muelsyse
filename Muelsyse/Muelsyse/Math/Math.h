#pragma once
#include <glm/glm.hpp>

namespace mul::Math 
{
	bool MUL_API decomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
}