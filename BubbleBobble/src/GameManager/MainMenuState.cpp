#include "JREngine/Input/InputManager.h"
#include "JREngine/Scene/SceneManager.h"
#include "JREngine/Core/ServiceLocator.h"
#include "JREngine/Physics/BoxPhysicsSystem.h"

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
    
        BoxPhysicsSystem& physicsSystem = static_cast<BoxPhysicsSystem&>(ServiceLocator::GetPhysicsSystem());
        physicsSystem.SetGravity(200.f);
        physicsSystem.SetWorldScale(0.1f);
    }
    void MainMenuState::OnEnter()
    {   
        Reset();
        InputManager::GetInstance().SetEnableActionMap(m_ActionMapIdx, true);
        SceneManager::GetInstance().SetNextScene(m_Name);
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
