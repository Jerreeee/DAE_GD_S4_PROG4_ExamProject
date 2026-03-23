#pragma once
#include <functional>

namespace JRE
{
    class IPredicate
    {
    public:
        virtual ~IPredicate() = default;
        virtual bool Evaluate() const = 0;

        operator std::function<bool()>() const
        {
            return [this] { return this->Evaluate(); };
        }
    };

    class TruePredicate : public IPredicate
    {
    public:
        virtual bool Evaluate() const override { return true; };
    };

    class FalsePredicate : public IPredicate
    {
    public:
        virtual bool Evaluate() const override { return false; };
    };

    using Predicate = std::function<bool()>;

    class State
    {
    public:
        virtual ~State() = default;

        virtual void OnEnter() {};
        virtual void OnExit() {};
        virtual void Update(float /*dt*/) {};
        virtual void FixedUpdate(float /*dt*/) {}; // Uses fixed dt
        virtual void OnEvent(Event&) {};
    };
}
