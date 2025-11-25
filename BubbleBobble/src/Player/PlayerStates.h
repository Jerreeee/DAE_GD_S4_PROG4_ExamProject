#pragma once
#include "JREngine/Core/Event.h"

namespace BubbleBobble
{
    class PlayerScriptComponent;

    enum class PlayerState
    {
        Mortal, Immortal, Dead
    };

    class PlayerStateBase
    {
    public:
        PlayerStateBase(PlayerScriptComponent& player) : m_Player{ &player } {};
        virtual ~PlayerStateBase() = default;

        virtual void OnEnter() {}
        virtual void OnExit() {}
        virtual void Update() {}
        virtual void FixedUpdate() {}
        virtual void OnEvent(JRE::EventInfo&) {}
    protected:
        PlayerScriptComponent* m_Player;
    };

    class PlayerState_Mortal final : public PlayerStateBase
    {
    public:
        PlayerState_Mortal(PlayerScriptComponent& player) : PlayerStateBase(player) {};
        void Update() override;
        void FixedUpdate() override;
        void OnEvent(JRE::EventInfo&) override;
    };

    class PlayerState_Immortal final : public PlayerStateBase
    {
    public:
        PlayerState_Immortal(PlayerScriptComponent& player, float time)
            : PlayerStateBase(player), m_TimeDefault{ time }, m_Time { time}
        {};
        void Update() override;
        void FixedUpdate() override;
    private:
        float m_TimeDefault{};
        float m_Time{};
    };

    class PlayerState_Dead final : public PlayerStateBase
    {
    public:
        PlayerState_Dead(PlayerScriptComponent& player) : PlayerStateBase(player) {};
        void OnEnter() override;
        void OnEvent(JRE::EventInfo&) override;
    };
}
