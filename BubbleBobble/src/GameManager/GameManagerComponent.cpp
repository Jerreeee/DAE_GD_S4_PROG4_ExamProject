#include "JREngine/Scene/Scene.h"
#include "JREngine/Scene/SceneManager.h"
#include "JREngine/Input/InputManager.h"

#include "GameManager/MainMenuState.h"
#include "GameManager/LoadingScreenState.h"
#include "GameManager/InGameState.h"
#include "GameManager/GameManagerComponent.h"
#include "Player/PlayerBuilder.h"

using namespace JRE::Input;

namespace BubbleBobble
{
	GameManagerComponent::GameManagerComponent(JRE::GameObject& gameObject, GameState startState)
		: ComponentBase(gameObject)
	{
		//Register player with the InputManager
		auto& im = InputManager::GetInstance();
		im.AddKeyboard();

		//Create all the states
		m_States.emplace_back(std::make_unique<MainMenuState>(*this));
		m_States.emplace_back(std::make_unique<LoadingScreenState>(*this));
		m_States.emplace_back(std::make_unique<InGameState>(*this));
		//Set active state
		m_pCurrentState = m_States[static_cast<size_t>(startState)].get();
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
		int idx = static_cast<int>(state);
		return m_States[idx]->GetName();
	}
	void GameManagerComponent::SetGameMode(GameMode mode)
	{
		m_GameMode = mode;

		switch (m_GameMode)
		{
		case GameMode::SinglePlayer:
		{
			//##################
			//Player 1
			//##################
			//auto player1 = std::make_unique<JRE::GameObject>("Player1");
			//Player::Builder()
			//	.SetAnimationPath("Data/Anims/P1.txt")
			//	.Build(player1);
			//scene.Add(std::move(player1));
		}
		}
	}
}
