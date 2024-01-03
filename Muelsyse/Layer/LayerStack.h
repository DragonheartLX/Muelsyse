#pragma once
#include "Core/Core.h"
#include "Layer/Layer.h"

#include <vector>

namespace mul 
{
	class MUL_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);
		void popLayer(Layer* layer);
		void popOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	private:
		std::vector<Layer*> m_Layers;
		unsigned int  m_LayerInsertIndex = 0;
	};

}