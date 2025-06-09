#pragma once
#include "JREngine/Scene/ComponentBase.h"

namespace JRE
{
	class GameObject;
	struct Input::ActionMap;
}

namespace BubbleBobble
{
	class PlayerScriptComponent;
	class PlayerControllerComponent : public JRE::ComponentBase
	{
	public:
		PlayerControllerComponent(JRE::GameObject& gameObject, size_t actinoMapIdx);

		void Update() override;
	private:
		PlayerScriptComponent* m_pPlayerScript{ nullptr };
		const JRE::Input::ActionMap* m_pActionMap{ nullptr };
	};
}
