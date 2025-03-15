#pragma once
#include <array>
#include <memory>

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

	struct Event
	{
		explicit Event(EventID _id);
		virtual ~Event();
		const EventID id;
		std::unique_ptr<EventArgs> args{};
	};
}