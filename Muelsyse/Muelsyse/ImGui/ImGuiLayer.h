#pragma once
#include "Muelsyse/Layer/Layer.h"
#include "Muelsyse/Event/ApplicationEvent.h"
#include "Muelsyse/Event/KeyEvent.h"
#include "Muelsyse/Event/MouseEvent.h"

namespace mul 
{
	class MUL_API ImGuiLayer: public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onImGuiRender() override;

		void begin();
		void end();

	private:
		float m_Time = 0.0f;
	};
}