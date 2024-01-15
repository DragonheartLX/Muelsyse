#pragma once
#include "Muelsyse/Camera/Camera.h"

namespace mul
{
	class MUL_API SceneCamera: public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };

		SceneCamera();
		virtual ~SceneCamera() = default;

		void setOrthographic(float size, float nearClip, float farClip);
		void setPerspective(float verticalFOV, float nearClip, float farClip);

		void setViewportSize(uint32_t width, uint32_t height);

		float getPerspectiveVerticalFOV() const { return m_PerspectiveFOV; }
		void setPerspectiveVerticalFOV(float verticalFov) { m_PerspectiveFOV = verticalFov; m_RecalculateProjection(); }
		float getPerspectiveNearClip() const { return m_PerspectiveNear; }
		void setPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; m_RecalculateProjection(); }
		float getPerspectiveFarClip() const { return m_PerspectiveFar; }
		void setPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; m_RecalculateProjection(); }

		float getOrthographicSize() const { return m_OrthographicSize; }
		void setOrthographicSize(float size) { m_OrthographicSize = size; m_RecalculateProjection(); }
		float getOrthographicNearClip() const { return m_OrthographicNear; }
		void setOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; m_RecalculateProjection(); }
		float getOrthographicFarClip() const { return m_OrthographicFar; }
		void setOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; m_RecalculateProjection(); }

		ProjectionType getProjectionType() const { return m_ProjectionType; }
		void setProjectionType(ProjectionType type) { m_ProjectionType = type; m_RecalculateProjection(); }
	private:
		void m_RecalculateProjection();

		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		float m_PerspectiveFOV = glm::radians(45.0f);
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

		float m_AspectRatio = 0.0f;
	};

}