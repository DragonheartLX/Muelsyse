#include "mulpch.h"
#include "Muelsyse/Camera/OrthographicCameraController.h"

#include "Muelsyse/Input/Input.h"
#include "Muelsyse/Input/KeyCode.h"

namespace mul
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation): 
        m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{

	}

	void OrthographicCameraController::onUpdate(Timestep ts)
	{
		MUL_PROFILE_FUNCTION();

		if (Input::isKeyPressed(MUL_KEY_A))
		{
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		else if (Input::isKeyPressed(MUL_KEY_D))
		{
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}

		if (Input::isKeyPressed(MUL_KEY_W))
		{
			m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		else if (Input::isKeyPressed(MUL_KEY_S))
		{
			m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}

		if (m_Rotation)
		{
			if (Input::isKeyPressed(MUL_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			if (Input::isKeyPressed(MUL_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;

			if (m_CameraRotation > 180.0f)
				m_CameraRotation -= 360.0f;
			else if (m_CameraRotation <= -180.0f)
				m_CameraRotation += 360.0f;

			m_Camera.setRotation(m_CameraRotation);
		}

		m_Camera.setPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::onEvent(Event& e)
	{
		MUL_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.dispatcher<MouseScrolledEvent>(MUL_BIND_EVENT_FUNC(OrthographicCameraController::onMouseScrolled));
		dispatcher.dispatcher<WindowResizeEvent>(MUL_BIND_EVENT_FUNC(OrthographicCameraController::onWindowResized));
	}

	bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& e)
	{
		MUL_PROFILE_FUNCTION();

		m_ZoomLevel -= e.getYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.setProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::onWindowResized(WindowResizeEvent& e)
	{
		MUL_PROFILE_FUNCTION();
		
		m_AspectRatio = (float)e.getWidth() / (float)e.getHeight();
		m_Camera.setProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

}