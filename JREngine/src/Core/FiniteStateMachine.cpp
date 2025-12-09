#include "JREngine/Core/FiniteStateMachine.h"

namespace JRE::FSM
{
    FiniteStateMachine::FiniteStateMachine(FSMState* startState)
        : m_pCurrentState(nullptr)
    {
    }

    void FiniteStateMachine::Start()
    {
        if (m_pCurrentState)
            m_pCurrentState->OnEnter();
    }

    void FiniteStateMachine::AddTransition(FSMState* start, FSMState* to, Predicate condition)
    {
        auto it = m_Transitions.find(start);
        if (it == m_Transitions.end())
            m_Transitions[start] = Transitions{};

        m_Transitions[start].push_back(std::make_pair(condition, to));
    }

    void FiniteStateMachine::Update(float dt)
    {
        if (!m_pCurrentState) return;

        auto it = m_Transitions.find(m_pCurrentState);
        if (it != m_Transitions.end())
        {
            for (auto& transition : it->second)
            {
                if (transition.first())
                {
                    ChangeState(transition.second);
                    break;
                }
            }
        }

        if (m_pCurrentState)
            m_pCurrentState->Update(dt);
    }

    void FiniteStateMachine::FixedUPdate(float dt)
    {
        if (m_pCurrentState)
            m_pCurrentState->FixedUpdate(dt);
    }

    void FiniteStateMachine::OnEvent(Event& event)
    {
        if (m_pCurrentState)
            m_pCurrentState->OnEvent(event);
    }

    void FiniteStateMachine::ChangeState(FSMState* newState)
    {
        if (!newState || newState == m_pCurrentState)
            return;

        if (m_pCurrentState)
            m_pCurrentState->OnExit();

        m_pCurrentState = newState;

        m_pCurrentState->OnEnter();
    }
}
