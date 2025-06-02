#include "JREngine/Input/InputManager.h"
#include "JREngine/Scene/SceneManager.h"

#include "GameManagerComponent.h"
#include "MainMenuState.h"

using namespace JRE;
using namespace JRE::Input;

namespace BubbleBobble
{
    MainMenuState::MainMenuState(GameManagerComponent& gameManagerComponent)
        : IGameState(gameManagerComponent)
    {
        //Setup controls
        auto& im = InputManager::GetInstance();
        m_ActionMapIdx = im.AddActionMap(0);
        ActionMap& actionMap = im.GetActionMap(m_ActionMapIdx);
        actionMap.BindCommand(std::make_unique<StartSinglePlayerCommand>(*this), std::make_unique<KeyboardBindingInfo>(KeyboardKey::Q, KeyState::DownThisFrame));
    }
    void MainMenuState::OnEnter()
    {
        InputManager::GetInstance().GetActionMap(m_ActionMapIdx).enabled = true;
        SceneManager::GetInstance().LoadScene("MainMenu");
    }
    GameState MainMenuState::Update()
    {
        if (m_StartSinglePlayer)
            return GameState::LoadingScreen;
        return GameState::None;
    }
    void MainMenuState::OnExit()
    {
        InputManager::GetInstance().GetActionMap(m_ActionMapIdx).enabled = false;
    }
    void MainMenuState::Reset()
    {
        m_StartSinglePlayer = false;
    }

}
