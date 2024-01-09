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

	void MUL_API ImGuiText(const char* fmt, ...);
	void MUL_API ImGuiBegin(const char* name, bool* p_open = NULL, int flags = 0);
	void MUL_API ImGuiColorEdit3(const char* label, float col[3], int flags = 0);
	void MUL_API ImGuiEnd();
}