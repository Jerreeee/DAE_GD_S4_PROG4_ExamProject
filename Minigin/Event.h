#pragma once
#include <array>
#include <memory>
#include <concepts>

namespace Engine
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
		~EventInfo();
		EventInfo(const EventInfo& eventInfo) = delete;
		EventInfo& operator=(const EventInfo& eventInfo) = delete;
		EventInfo(EventInfo&& eventInfo) noexcept;
		EventInfo& operator=(EventInfo&& eventInfo) noexcept;

		EventID GetID() const { return m_ID; };

		template<typename EventName>
		inline typename EventName::Args& GetArgs()
		{
			return *static_cast<typename EventName::Args*>(m_Args.get());
		}
	private:
		explicit EventInfo(EventID id);

		EventID m_ID{};
		std::unique_ptr<EventArgs> m_Args{};

		template<typename EventName, typename... ConstructorArgs>
		requires std::constructible_from<typename EventName::Args, ConstructorArgs...>
		friend EventInfo CreateEvent(ConstructorArgs&&... args);
	};

	template<typename EventName, typename... ConstructorArgs>
	requires std::constructible_from<typename EventName::Args, ConstructorArgs...>
	EventInfo CreateEvent(ConstructorArgs&&... args)
	{
		EventInfo e(EventName::ID);
		e.m_Args = std::make_unique<typename EventName::Args>(std::forward<ConstructorArgs>(args)...);
		return e;
	}

	//template<typename EventName>
	//EventInfo CreateEvent()
	//{
	//	EventInfo e(EventName::ID);
	//	e.m_Args = std::make_unique<typename EventName::Args>();
	//	return e;
	//}
}