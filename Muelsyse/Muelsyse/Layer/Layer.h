#pragma once
#include "Muelsyse/Core/Core.h"
#include "Muelsyse/Event/Event.h"
#include "Muelsyse/Core/Timestep.h"

namespace mul 
{
	class MUL_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate(Timestep ts) {}
		virtual void onImGuiRender() {}
		virtual void onEvent(Event& event) {}

		const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}