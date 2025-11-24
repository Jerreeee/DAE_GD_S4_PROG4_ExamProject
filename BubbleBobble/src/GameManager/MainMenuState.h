#pragma once
#include "JREngine/Core/Command.h"
#include "JREngine/Asset/Asset.h"

#include "GameManager/IGameState.h"

namespace JRE
{
	class ISoundClip;
}

namespace BubbleBobble
{
	class MainMenuState final : public IGameState
	{
	public:
		MainMenuState(GameManagerComponent& gameManagerComponent);

		virtual void OnEnter() override;
		virtual GameState Update() override;
		virtual void OnExit() override;
	private:
		void Reset();

		bool m_StartSinglePlayer{ false };
		size_t m_ActionMapIdx{};
		JRE::AssetRef<JRE::ISoundClip> m_OpeningSoundRef{};
		JRE::AssetRef<JRE::ISoundClip> m_SelectSoundRef{};
	};
}
