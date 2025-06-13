#include "JREngine/Input/InputManager.h"
#include "JREngine/Scene/GameObject.h"

#include "Player/PlayerScriptComponent.h"
#include "Player/PlayerController.h"

using namespace JRE;
using namespace JRE::Input;

namespace BubbleBobble
{
	PlayerControllerComponent::PlayerControllerComponent(JRE::GameObject& gameObject, size_t actinoMapIdx)
		: ComponentBase(gameObject)
		, m_pPlayerScript{ GetGameObject().GetComponent<PlayerScriptComponent>() }
		, m_pActionMap{ &InputManager::GetInstance().GetActionMap(actinoMapIdx) }
	{
		assert(m_pPlayerScript && "PlayerController::PlayerController() | No PlayerScriptComponent found on gameObject");
		assert(m_pActionMap && "PlayerController::PlayerController() | Invalid actionMapIdx");
	}
	void PlayerControllerComponent::Update()
	{
		if (!m_pPlayerScript)
			return;

		auto& input = InputManager::GetInstance();
		if (input.IsBindingActive(*m_pActionMap, "MoveLeft"))
			m_pPlayerScript->Move(-1);
		if (input.IsBindingActive(*m_pActionMap, "MoveRight"))
			m_pPlayerScript->Move(1);
		if (input.IsBindingActive(*m_pActionMap, "Jump"))
			m_pPlayerScript->Jump();
		if (input.IsBindingActive(*m_pActionMap, "Shoot"))
			m_pPlayerScript->Shoot();
	}
}
