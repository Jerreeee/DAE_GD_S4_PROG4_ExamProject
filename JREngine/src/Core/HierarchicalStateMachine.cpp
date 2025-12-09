#include "JREngine/Core/HierarchicalStateMachine.h"

namespace JRE::HSM
{
    HierarchicalStateMachine::HierarchicalStateMachine(HSMState* const startState)
    {
        m_pCurrentState = startState;
    }

    void HierarchicalStateMachine::SetParent(HSMState* const child, HSMState* const parent)
    {
        m_ParentMap[child] = parent;
    }

    void HierarchicalStateMachine::AddTransition(HSMState* const fromState, HSMState* const toState, Predicate predicate)
    {
        bool isInward = IsChildOf(toState, fromState);
        m_Transitions[fromState].push_back({ predicate, toState, isInward ? Transition::Dir::Inward : Transition::Dir::Outward });
    }

    void HierarchicalStateMachine::Start()
    {
        if (m_pCurrentState)
            m_pCurrentState->OnEnter();
    }

    void HierarchicalStateMachine::Update(float dt)
    {
        UpdateParents(m_pCurrentState, dt);
        m_pCurrentState->Update(dt);

        HSMState* newState = FindStateToTransitionTo(m_pCurrentState);
        if (newState)
            ChangeState(newState);
    }

    void HierarchicalStateMachine::FixedUpdate(float dt)
    {
        m_pCurrentState->FixedUpdate(dt);
    }

    void HierarchicalStateMachine::OnEvent(Event& event)
    {
        m_pCurrentState->OnEvent(event);
    }

    HSMState* HierarchicalStateMachine::FindStateToTransitionTo(HSMState* const fromState)
    {
        m_pFromState = fromState;

        HSMState* newState = RecursiveTrySearchNextState(fromState);
        if (newState)
            return newState;

        HSMState* parent = GetParent(fromState);
        while (parent)
        {
            newState = RecursiveTrySearchNextState(parent);
            if (newState)
                return newState;
            parent = GetParent(parent);
        }
        return nullptr;
    }

    HSMState* HierarchicalStateMachine::RecursiveTrySearchNextState(HSMState* state, Transition::Dir dir) const
    {
        const Transition* transition = TrySearchValidTransition(state, dir);
        if (!transition)
            return nullptr;

        bool isParent = IsParent(transition->pToState);
        if (!isParent)
            return transition->pToState;

        if (transition->direction == Transition::Dir::Outward)
            dir = Transition::Dir::Inward;

        return RecursiveTrySearchNextState(transition->pToState, dir);
    }

    const Transition* HierarchicalStateMachine::TrySearchValidTransition(HSMState* state, Transition::Dir direction) const
    {
        auto it = m_Transitions.find(state);
        if (it == m_Transitions.end())
            return nullptr;

        const auto& transitions = it->second;

        for (const auto& transition : transitions)
        {
            if (transition.direction == direction &&
                transition.pToState != m_pFromState &&
                transition.predicate())
            {
                return &transition;
            }
        }

        return nullptr;
    }

    void HierarchicalStateMachine::ChangeState(HSMState* const newState)
    {
        if (!newState || m_pCurrentState == newState)
            return;

        HSMState* oldState = m_pCurrentState;
        m_pCurrentState->OnExit();

        HSMState* commonParent = nullptr;
        HSMState* parent = GetParent(oldState);

        while (parent)
        {
            if (IsChildOf(newState, parent))
            {
                commonParent = parent;
                break;
            }
            parent->OnExit();
            parent = GetParent(parent);
        }

        std::vector<HSMState*> newParents;
        parent = GetParent(newState);

        while (parent && parent != commonParent)
        {
            newParents.push_back(parent);
            parent = GetParent(parent);
        }

        for (int i = static_cast<int>(newParents.size()) - 1; i >= 0; --i)
            newParents[i]->OnEnter();

        m_pCurrentState = newState;
        m_pCurrentState->OnEnter();
    }

    void HierarchicalStateMachine::UpdateParents(HSMState* state, float deltaTime)
    {
        HSMState* parent = GetParent(state);
        while (parent)
        {
            parent->Update(deltaTime);
            parent = GetParent(parent);
        }
    }

    HSMState* HierarchicalStateMachine::GetParent(HSMState* state) const
    {
        auto it = m_ParentMap.find(state);
        return (it != m_ParentMap.end()) ? it->second : nullptr;
    }

    bool HierarchicalStateMachine::IsChildOf(HSMState* child, HSMState* possibleParent) const
    {
        HSMState* parent = GetParent(child);
        while (parent)
        {
            if (parent == possibleParent)
                return true;
            parent = GetParent(parent);
        }
        return false;
    }

    bool HierarchicalStateMachine::IsParent(HSMState* state) const
    {
        for (auto& pair : m_ParentMap)
        {
            if (pair.second == state)
                return true;
        }
        return false;
    }
}
