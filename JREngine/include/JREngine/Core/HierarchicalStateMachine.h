#pragma once
#include <map>
#include <vector>
#include <functional>
#include <cassert>

#include "JREngine/Core/Event.h"
#include "JREngine/Core/State.h"

namespace JRE::HSM
{
    using HSMState = State;

    struct Transition
    {
        enum class Dir { Outward, Inward };

        Predicate predicate;
        HSMState* pToState = nullptr;
        Dir direction = Dir::Outward;
    };

    class HierarchicalStateMachine
    {
    public:
        explicit HierarchicalStateMachine(HSMState* startState);
        ~HierarchicalStateMachine() = default;

        void Start();
        void Update(float deltaTime);
        void FixedUpdate(float deltaTime);
        void OnEvent(Event& event);

        void AddTransition(HSMState* from, HSMState* to, Predicate predicate);
        void SetParent(HSMState* child, HSMState* parent);

    private:
        HSMState* FindStateToTransitionTo(HSMState* fromState);
        HSMState* RecursiveTrySearchNextState(HSMState* state, Transition::Dir dir = Transition::Dir::Outward) const;
        const Transition* TrySearchValidTransition(HSMState* state, Transition::Dir dir) const;

        void ChangeState(HSMState* newState);
        void UpdateParents(HSMState* state, float dt);

        HSMState* GetParent(HSMState* state) const;
        bool IsChildOf(HSMState* child, HSMState* parent) const;
        bool IsParent(HSMState* state) const;

        std::map<HSMState*, HSMState*> m_ParentMap;
        std::map<HSMState*, std::vector<Transition>> m_Transitions;

        HSMState* m_pCurrentState = nullptr;
        HSMState* m_pFromState = nullptr;
    };
}
