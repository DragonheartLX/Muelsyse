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
		virtual void onEvent(Event& e) override;

		void begin();
		void end();

		void blockEvents(bool block) { m_BlockEvents = block; }

		void setDarkThemeColors();
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};
}