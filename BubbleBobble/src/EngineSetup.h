#pragma once
#include <cstdint>

namespace BubbleBobble
{
	struct CollisionLayer
	{
		static constexpr uint32_t Friendly = 1 << 0;
		static constexpr uint32_t Enemy = 1 << 1;
		static constexpr uint32_t StaticLevel = 1 << 2;
		static constexpr uint32_t Platform = 1 << 3;
	};

	struct CollisionMask
	{
		static constexpr uint32_t Friendly = CollisionLayer::Enemy | CollisionLayer::StaticLevel | CollisionLayer::Platform;
		static constexpr uint32_t Enemy = CollisionLayer::Friendly | CollisionLayer::StaticLevel | CollisionLayer::Platform;
		static constexpr uint32_t StaticLevel = CollisionLayer::Friendly | CollisionLayer::Enemy;
		static constexpr uint32_t Platform = CollisionLayer::Friendly | CollisionLayer::Enemy;
	};

	struct PersistenceLayer
	{
		static constexpr uint32_t None = 1 << 0;
		static constexpr uint32_t InGame = 1 << 1;
		static constexpr uint32_t Global = 1 << 2;
	};

	struct PersistenceMask
	{
		static constexpr uint32_t InGameScene = PersistenceLayer::InGame | PersistenceLayer::Global;
		static constexpr uint32_t MenuScene = PersistenceLayer::Global;
	};
}
