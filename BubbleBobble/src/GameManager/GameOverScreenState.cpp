#include "JREngine/Input/InputManager.h"
#include "JREngine/Scene/SceneManager.h"

#include "GameManager/GameOverScreenState.h"

using namespace JRE;
using namespace JRE::Input;

namespace BubbleBobble
{
	GameOverScreenState::GameOverScreenState(GameManagerComponent& gameManagerComponent)
		: IGameState(gameManagerComponent, "GameOverScreen")
	{
		//Setup controls
		auto& im = InputManager::GetInstance();
		m_ActionMapIdx = im.AddActionMap({ 0 });
		im.BindCommand(m_ActionMapIdx, "ReturnToMainMenu", std::make_unique<ReturnToMainMenuCommand>(*this), std::make_unique<ControllerBindingInfo>(ControllerButton::DPAD_LEFT, ButtonState::DownThisFrame));
		InputManager::GetInstance().SetEnableActionMap(m_ActionMapIdx, false);
	}
	void GameOverScreenState::OnEnter()
	{
		auto& sm = SceneManager::GetInstance();
		InputManager::GetInstance().SetEnableActionMap(m_ActionMapIdx, true);
		sm.SetNextScene(m_Name);
	}
	GameState GameOverScreenState::Update()
	{
		if (m_ReturnToMainMenu)
			return GameState::MainMenu;
		return GameState::None;
	}
	void GameOverScreenState::OnExit()
	{
		InputManager::GetInstance().SetEnableActionMap(m_ActionMapIdx, false);
	}
}
