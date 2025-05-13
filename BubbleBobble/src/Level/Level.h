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
		struct DrawInfo
		{
			size_t spriteIdx;
			glm::vec2 pos;
		};

		Level(const std::filesystem::path& path);

		const std::vector<DrawInfo>& GetDrawInfo() const;

		static constexpr std::string_view GetStaticType() { return "BubbleBobble::Level"; };
		virtual std::string_view GetType() const override { return GetStaticType(); };
	private:
		void LoadSprites(const std::filesystem::path& path);
		void LoadTileData(const std::filesystem::path& path);

		std::vector<DrawInfo> m_DrawInfo{};
		std::vector<JRE::AssetRef<JRE::Sprite>> m_Sprites{};
		std::vector<std::string> m_SpriteNames{};
	};
}
