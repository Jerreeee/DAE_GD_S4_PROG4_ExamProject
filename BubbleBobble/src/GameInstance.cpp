#include "JREngine/Scene/Scene.h"
#include "JREngine/Scene/SceneManager.h"
#include "JREngine/Input/InputManager.h"

#include "GameInstance.h"

namespace BubbleBobble
{
	void GameInstance::Init(GameState)
	{
		//auto& inputManager = JRE::Input::InputManager::GetInstance();
		//auto setLoadingScreenCommand = std::make_unique<SetGameStateCommand>(m_GameState, GameState::LoadingScreen);
		//inputManager.BindCommand(0, std::move(setLoadingScreenCommand), JRE::Input::ControllerBindingInfo{ JRE::Input::ControllerButton::DPAD_LEFT, JRE::Input::ButtonState::Pressed });
		//JRE::SceneManager::GetInstance().LoadScene(m_SceneNames[static_cast<int>(state)]);
	}
	void GameInstance::OnNotify(JRE::EventInfo&)
	{
	}
}
