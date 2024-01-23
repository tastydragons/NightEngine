#pragma once

#include "NightEngine/Core/Layer.h"

#include "NightEngine/Events/ApplicationEvent.h"
#include "NightEngine/Events/KeyEvent.h"
#include "NightEngine/Events/MouseEvent.h"

namespace Night
{
	class NIGHT_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;

	};
}