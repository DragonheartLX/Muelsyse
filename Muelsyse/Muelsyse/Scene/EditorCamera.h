#pragma once
#include "Muelsyse/Camera/Camera.h"
#include "Muelsyse/Core/Timestep.h"
#include "Muelsyse/Event/Event.h"
#include "Muelsyse/Event/MouseEvent.h"

#include <glm/glm.hpp>

namespace mul 
{
	class MUL_API EditorCamera: public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void onUpdate(Timestep ts);
		void onEvent(Event& e);

		inline float getDistance() const { return m_Distance; }
		inline void setDistance(float distance) { m_Distance = distance; }

		inline void setViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; m_UpdateProjection(); }

		const glm::mat4& getViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 getViewProjection() const { return m_Projection * m_ViewMatrix; }

		glm::vec3 getUpDirection() const;
		glm::vec3 getRightDirection() const;
		glm::vec3 getForwardDirection() const;
		const glm::vec3& getPosition() const { return m_Position; }
		glm::quat getOrientation() const;

		float getPitch() const { return m_Pitch; }
		float getYaw() const { return m_Yaw; }
	private:
		void m_UpdateProjection();
		void m_UpdateView();

		bool m_OnMouseScroll(MouseScrolledEvent& e);

		void m_MousePan(const glm::vec2& delta);
		void m_MouseRotate(const glm::vec2& delta);
		void m_MouseZoom(float delta);

		glm::vec3 m_CalculatePosition() const;

		std::pair<float, float> m_PanSpeed() const;
		float m_RotationSpeed() const;
		float m_ZoomSpeed() const;

		float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

		glm::mat4 m_ViewMatrix;
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };

		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

		float m_Distance = 10.0f;
		float m_Pitch = 0.0f, m_Yaw = 0.0f;

		float m_ViewportWidth = 1280, m_ViewportHeight = 720;
	};
}