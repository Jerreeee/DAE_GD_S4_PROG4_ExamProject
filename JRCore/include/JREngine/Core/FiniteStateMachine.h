#pragma once
#include <map>
#include <vector>
#include <functional>

#include "JREngine/Core/Event.h"
#include "JREngine/Core/State.h"

namespace JRE::FSM
{
    using FSMState = State;

    class FiniteStateMachine
    {
    public:
        explicit FiniteStateMachine(FSMState* startState);

        void Start();
        void Update(float dt);
        void FixedUPdate(float dt);
        void OnEvent(Event& event);

        void AddTransition(FSMState* start, FSMState* to, Predicate condition);
    private:
        void ChangeState(FSMState* newState);

        using Transition = std::pair<Predicate, FSMState*>;
        using Transitions = std::vector<Transition>;

        std::map<FSMState*, Transitions> m_Transitions{};
        FSMState* m_pCurrentState{};
    };
}
