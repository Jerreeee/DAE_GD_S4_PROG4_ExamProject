#include "JREngine/Scene/Scene.h"
#include "JREngine/Scene/SceneManager.h"
#include "JREngine/Input/InputManager.h"

#include "GameManager/MainMenuState.h"
#include "GameManager/GameManagerComponent.h"

using namespace JRE::Input;

namespace BubbleBobble
{
	const std::vector<std::string> GameManagerComponent::s_StateNames{ "MainMenu", "LoadingScreen", "Playing", "GameOver" };

	GameManagerComponent::GameManagerComponent(JRE::GameObject& gameObject, GameState startState)
		: ComponentBase(gameObject)
	{
		//Create all the states
		m_States.emplace_back(std::make_unique<MainMenuState>(*this));
		//Set active state
		m_pCurrentState = m_States[static_cast<size_t>(startState)].get();


		//Register player with the InputManager
		auto& im = InputManager::GetInstance();
		size_t keyboardIdx = im.AddKeyboard();
		im.AddPlayer(keyboardIdx);
	}
	void GameManagerComponent::Start()
	{
		m_pCurrentState->OnEnter();
	}
	void GameManagerComponent::Update()
	{
		GameState newState = m_pCurrentState->Update();
		if (newState == GameState::None)
			return;

		m_pCurrentState->OnExit();
		m_pCurrentState = m_States[static_cast<size_t>(newState)].get();
		m_pCurrentState->OnEnter();
	}
	std::string GameManagerComponent::GetStateName(GameState state)
	{
		return s_StateNames[static_cast<size_t>(state)];
	}
}
