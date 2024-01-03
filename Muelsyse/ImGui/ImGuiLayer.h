#pragma once
#include "Layer/Layer.h"
#include "Event/ApplicationEvent.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"

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

		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
	};
}