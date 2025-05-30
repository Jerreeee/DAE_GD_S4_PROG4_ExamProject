#pragma once
#include <vector>
#include <filesystem>
#include <glm.hpp>
#include "JREngine/Asset/Asset.h"

namespace JRE
{
	class Sprite;
}

namespace BubbleBobble
{
	class Level final : public JRE::Asset
	{
	public:
		struct SpritePos
		{
			size_t spriteIdx;
			glm::vec2 pos;
		};

		Level() = default;

		void Render();

		void SetSprites(const std::vector<JRE::AssetRef<JRE::Sprite>>& sprites);
		void SetSpritePositions(const std::vector<Level::SpritePos>& spritePositions);

		const std::vector<SpritePos>& GetDrawInfo() const;

		static constexpr std::string_view GetStaticType() { return "BubbleBobble::Level"; };
		virtual std::string_view GetType() const override { return GetStaticType(); };
	private:
		std::vector<SpritePos> m_SpritePositions{};
		std::vector<JRE::AssetRef<JRE::Sprite>> m_Sprites{};
		std::vector<std::string> m_SpriteNames{};
	};
}
