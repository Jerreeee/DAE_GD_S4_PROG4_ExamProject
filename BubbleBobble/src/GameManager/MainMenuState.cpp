#include "JREngine/Input/InputManager.h"
#include "JREngine/Scene/SceneManager.h"
#include "JREngine/Core/ServiceLocator.h"

#include "JREngine/Physics/BoxPhysicsSystem.h"
#include "JREngine/Asset/ResourceManager.h"
#include "JREngine/Audio/ISoundSystem.h"

#include "GameManagerComponent.h"
#include "MainMenuState.h"

#include <iostream>

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
        im.BindCommand(m_ActionMapIdx, "StartSinglePlayer",
            [this] { m_StartSinglePlayer = true; },
            std::make_unique<ControllerBindingInfo>(ControllerButton::DPAD_LEFT, ButtonState::DownThisFrame));

        InputManager::GetInstance().SetEnableActionMap(m_ActionMapIdx, false);

        BoxPhysicsSystem& physicsSystem = static_cast<BoxPhysicsSystem&>(ServiceLocator::GetPhysicsSystem());
        physicsSystem.SetGravity(200.f);
        physicsSystem.SetWorldScale(0.1f);

        m_OpeningSoundRef = ResourceManager::GetAsset<ISoundClip>(GetRegisteredHandle("HUD/Opening.wav"));
        m_SelectSoundRef = ResourceManager::GetAsset<ISoundClip>(GetRegisteredHandle("HUD/Select.wav"));
    }
    void MainMenuState::OnEnter()
    {   
        Reset();
        InputManager::GetInstance().SetEnableActionMap(m_ActionMapIdx, true);
        SceneManager::GetInstance().SetNextScene(m_Name);
        ServiceLocator::GetSoundSystem().Play(m_OpeningSoundRef);
    }
    GameState MainMenuState::Update()
    {
        if (m_StartSinglePlayer)
        {
            m_GameManagerComponent.SetGameMode(GameMode::SinglePlayer);
            ServiceLocator::GetSoundSystem().Play(m_SelectSoundRef);
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
