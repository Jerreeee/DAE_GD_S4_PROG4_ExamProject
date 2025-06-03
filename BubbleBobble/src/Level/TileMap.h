#pragma once
#include <vector>
#include <filesystem>
#include "JREngine/Asset/Asset.h"
#include "JREngine/Math/Rect.h"

namespace JRE
{
	class Sprite;
}

namespace BubbleBobble
{
	class TileMap final : public JRE::Asset
	{
	public:
		struct CollisionInfo
		{
			JRE::Region region;
			bool isPlatform;
		};

		struct SpritePos
		{
			size_t spriteIdx;
			glm::vec2 pos;
		};

		TileMap() = default;

		void Render();

		void SetSprites(const std::vector<JRE::AssetRef<JRE::Sprite>>& sprites);
		void SetSpritePositions(const std::vector<TileMap::SpritePos>& spritePositions);
		void SetCollisionRects(const std::vector<CollisionInfo>& rects);

		const std::vector<SpritePos>& GetDrawInfo() const;

		static constexpr std::string_view GetStaticType() { return "BubbleBobble::TileMap"; };
		virtual std::string_view GetType() const override { return GetStaticType(); };
	private:
		std::vector<SpritePos> m_SpritePositions{};
		std::vector<JRE::AssetRef<JRE::Sprite>> m_Sprites{};
		std::vector<CollisionInfo> m_CollisionRects{};
	};
}
