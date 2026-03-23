#pragma once
#include <array>
#include <memory>
#include <concepts>
#include <cassert>
#include <functional>
#include <type_traits>
#include "JREngine/Core/Observer.h"

namespace JRE
{
	using EventID = uint32_t;

	constexpr EventID HashEventID(const char* str)
	{
		uint32_t hash = 0;
		while (*str)
		{
			hash = *str++ + (hash << 6) + (hash << 16) - hash;
		}
		return hash;
	};

	struct EventArgs
	{
		virtual ~EventArgs() = default;
	};

	class EventInfo final
	{
	public:
		EventInfo(EventID id, std::unique_ptr<EventArgs> args);

		EventID GetID() const { return m_ID; }

		template<typename EventType>
		EventType::Args& GetArgs()
		{
			assert(m_Args && "EventInfo: Args is null");
			assert(dynamic_cast<EventType::Args*>(m_Args.get()) && "EventInfo: type mismatch");
			return *static_cast<typename EventType::Args*>(m_Args.get());
		}

	private:
		EventID m_ID;
		std::unique_ptr<EventArgs> m_Args;
	};

	template<typename EventType, typename... Args>
		requires std::constructible_from<typename EventType::Args, Args...>
	inline EventInfo CreateEvent(Args&&... args)
	{
		return EventInfo(EventType::ID, std::make_unique<typename EventType::Args>(std::forward<Args>(args)...));
	}

	class Event final
	{
	public:
		Event() = default;
		~Event();
		Event(const Event&) = delete;
		Event& operator=(const Event&) = delete;
		Event(Event&&) noexcept = delete;
		Event& operator=(Event&&) noexcept = delete;

		std::shared_ptr<EventConnection> AddObserver(std::function<void(EventInfo&)> callback) const;
		void RemoveObserver(EventConnection* conn) const;
		void Notify(EventInfo& event) const;
	private:
		mutable std::unique_ptr<Observable> m_Observable{ nullptr };
	};

	struct EventConnection
	{
		const Event* event = nullptr;
		std::function<void(EventInfo&)> callback;

		EventConnection(const Event* e, std::function<void(EventInfo&)> cb)
			: event{ e }, callback{ std::move(cb) } {}

		~EventConnection() { if (event) event->RemoveObserver(this); }

		EventConnection(const EventConnection&) = delete;
		EventConnection& operator=(const EventConnection&) = delete;
		EventConnection(EventConnection&&) = delete;
		EventConnection& operator=(EventConnection&&) = delete;
	};
}
