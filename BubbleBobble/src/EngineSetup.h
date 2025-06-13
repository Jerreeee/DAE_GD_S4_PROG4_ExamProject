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
		static constexpr uint32_t Bubble = 1 << 4;
	};

	struct CollisionMask
	{
		static constexpr uint32_t Friendly = CollisionLayer::Enemy | CollisionLayer::StaticLevel | CollisionLayer::Platform | CollisionLayer::Bubble;
		static constexpr uint32_t Enemy = CollisionLayer::Friendly | CollisionLayer::StaticLevel | CollisionLayer::Platform | CollisionLayer::Bubble;
		static constexpr uint32_t DynamicGameObject = CollisionLayer::Friendly | CollisionLayer::Enemy;
		static constexpr uint32_t StaticLevel = CollisionLayer::Friendly | CollisionLayer::Enemy;
		static constexpr uint32_t Platform = CollisionLayer::Friendly | CollisionLayer::Enemy;
		static constexpr uint32_t Bubble = CollisionLayer::Friendly | CollisionLayer::Enemy;
	};

	struct PersistenceLayer
	{
		static constexpr uint32_t SceneLocal = JRE::PersistenceLayer::SceneLocal;
		static constexpr uint32_t SceneTemporary = JRE::PersistenceLayer::SceneTemporary;

		static constexpr uint32_t Global = 1 << 2;
		static constexpr uint32_t Levels = 1 << 3;
	};

	struct PersistenceMask
	{
		static constexpr uint32_t PlayerAndUI = PersistenceLayer::Levels;
		static constexpr uint32_t LevelScene = PersistenceLayer::Global | PersistenceLayer::Levels;
		static constexpr uint32_t MenuScene = PersistenceLayer::Global | PersistenceLayer::SceneLocal;
	};
}
