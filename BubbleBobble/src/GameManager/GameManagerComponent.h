#pragma once
#include <string>
#include "JREngine/Scene/ComponentBase.h"
#include "JREngine/Asset/Asset.h"

#include "GameManager/IGameState.h"

namespace JRE
{
	class ISoundMusic;
}

namespace BubbleBobble
{
	enum class GameMode
	{
		SinglePlayer, MultiPlayer, Versus
	};

	class GameManagerComponent final : public JRE::ComponentBase
	{
	public:
		GameManagerComponent(JRE::GameObject& gameObject, GameState startState);

		virtual void Start() override;
		virtual void Update() override;

		void PlayMusic(bool play);

		std::string GetStateName(GameState state);
		void SetGameMode(GameMode mode);
		GameMode GetGameMode() const { return m_GameMode; };
	private:
		IGameState* m_pCurrentState{};
		std::vector<std::unique_ptr<IGameState>> m_States{};
		GameState m_GameState{ GameState::MainMenu };
		GameMode m_GameMode{ GameMode::SinglePlayer };

		JRE::AssetRef<JRE::ISoundMusic> m_MusicRef{};
	};
}
