#include "JREngine/Input/InputManager.h"
#include "JREngine/Scene/SceneManager.h"
#include "JREngine/Core/ServiceLocator.h"

#include "JREngine/Asset/AssetImporter.h"
#include "JREngine/Asset/SoundClipImporter.h"
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

        AssetHandle openingSoundHandle = AssetImporter::GetInstance().ImportAsset(std::move(JRE::SoundClipImporter("HUD/Opening.wav")));
        m_OpeningSoundRef = ResourceManager::GetAsset<ISoundClip>(openingSoundHandle);
        AssetHandle selectSoundHandle = AssetImporter::GetInstance().ImportAsset(std::move(JRE::SoundClipImporter("HUD/Select.wav")));
        m_SelectSoundRef = ResourceManager::GetAsset<ISoundClip>(selectSoundHandle);
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
