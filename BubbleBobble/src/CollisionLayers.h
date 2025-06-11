#pragma once
#include <cstdint>

namespace BubbleBobble
{
	struct CollisionLayer
	{
		static constexpr uint32_t Friendly = 1 << 0;
		static constexpr uint32_t Enemy = 1 << 1;
		static constexpr uint32_t StaticLevel = 1 << 2;
	};

	struct CollisionMask
	{
		static constexpr uint32_t Friendly = CollisionLayer::Enemy | CollisionLayer::StaticLevel;
		static constexpr uint32_t Enemy = CollisionLayer::Friendly | CollisionLayer::StaticLevel;
		static constexpr uint32_t StaticLevel = CollisionLayer::Friendly | CollisionLayer::Enemy;
	};
}
