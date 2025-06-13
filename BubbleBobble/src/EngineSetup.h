#pragma once
#include <cstdint>
#include "JREngine/Scene/PersistenceLayer.h"

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
		static constexpr uint32_t SceneLocal = JRE::PersistenceLayer::SceneLocal;
		static constexpr uint32_t None = JRE::PersistenceLayer::None;
		static constexpr uint32_t AccrossScenes = JRE::PersistenceLayer::AcrossScenes;

		static constexpr uint32_t AllLevels = 1 << 2;
	};

	struct PersistenceMask
	{
		static constexpr uint32_t LevelScene = PersistenceLayer::AccrossScenes | PersistenceLayer::AllLevels;
		static constexpr uint32_t MenuScene = PersistenceLayer::AccrossScenes | PersistenceLayer::SceneLocal;
	};
}
