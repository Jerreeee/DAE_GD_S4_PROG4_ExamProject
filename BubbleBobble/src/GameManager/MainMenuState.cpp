#include "JREngine/Input/InputManager.h"
#include "JREngine/Scene/SceneManager.h"

#include "GameManagerComponent.h"
#include "MainMenuState.h"

using namespace JRE;
using namespace JRE::Input;

namespace BubbleBobble
{
    MainMenuState::MainMenuState(GameManagerComponent& gameManagerComponent)
        : IGameState(gameManagerComponent, "MainMenu")
    {
        //Setup controls
        auto& im = InputManager::GetInstance();
        m_ActionMapIdx = im.AddActionMap({ 0 });
        im.BindCommand(m_ActionMapIdx, "StartSinglePlayer", std::make_unique<StartSinglePlayerCommand>(*this), std::make_unique<KeyboardBindingInfo>(KeyboardKey::Q, KeyState::DownThisFrame));
    }
    void MainMenuState::OnEnter()
    {   
        Reset();
        InputManager::GetInstance().SetEnableActionMap(m_ActionMapIdx, true);
        SceneManager::GetInstance().LoadScene(m_Name);
    }
    GameState MainMenuState::Update()
    {
        if (m_StartSinglePlayer)
        {
            m_GameManagerComponent.SetGameMode(GameMode::SinglePlayer);
            return GameState::LoadingScreen;
        }
        return GameState::None;
    }
    void MainMenuState::OnExit()
    {
        InputManager::GetInstance().SetEnableActionMap(m_ActionMapIdx, false);
    }
    void MainMenuState::Reset()
    {
        m_StartSinglePlayer = false;
    }

}
