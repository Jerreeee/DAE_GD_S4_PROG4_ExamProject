#pragma once
#include <vector>
#include <limits>
#include <filesystem>
#include "JREngine/Asset/Asset.h"
#include "JREngine/Math/Rect.h"
#include "JREngine/Physics/ICollider.h"
#include "glm.hpp"

namespace JRE
{
	class Sprite;
}

namespace BubbleBobble
{
	class TileMap final : public JRE::Asset
	{
	public:
		struct ColliderInfo
		{
			JRE::BoxShape boxShape;
			bool isPlatform;
		};
		struct SpritePos
		{
			size_t spriteIdx;
			glm::vec2 pos;
		};

		TileMap();

		void Render();

		void SetSprites(const std::vector<JRE::AssetRef<JRE::Sprite>>& sprites);
		void SetSpritePositions(const std::vector<TileMap::SpritePos>& spritePositions);
		void SetCollisionRects(const std::vector<ColliderInfo>& rects);

		const std::vector<ColliderInfo>& GetCollisionRects() const { return m_CollisionRects; };
		const std::vector<SpritePos>& GetDrawInfo() const;

		static constexpr std::string_view GetStaticType() { return "BubbleBobble::TileMap"; };
		virtual std::string_view GetType() const override { return GetStaticType(); };
	private:
		std::vector<SpritePos> m_SpritePositions{};
		std::vector<JRE::AssetRef<JRE::Sprite>> m_Sprites{};
		uint32_t m_StaticCollisionGruop{};
		std::vector<ColliderInfo> m_CollisionRects{};
	};
}
