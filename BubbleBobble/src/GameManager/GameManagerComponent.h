#pragma once
#include <string>
#include "JREngine/Scene/ComponentBase.h"

#include "GameManager/IGameState.h"

namespace BubbleBobble
{
	class GameManagerComponent final : public JRE::ComponentBase
	{
	public:
		GameManagerComponent(JRE::GameObject& gameObject, GameState startState);

		virtual void Start() override;
		virtual void Update() override;

		std::string GetStateName(GameState state);
	private:
		IGameState* m_pCurrentState{};
		std::vector<std::unique_ptr<IGameState>> m_States{};
		GameState m_GameState{ GameState::MainMenu };
	};
}
