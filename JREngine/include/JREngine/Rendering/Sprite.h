#pragma once
#include <memory>
#include "JREngine/Resources/Asset.h"
#include "JREngine/Math/Rect.h"

namespace JRE
{
	class Texture2D;
	class Sprite final : public Asset
	{
	public:
		struct PathBuilder
		{
			static std::filesystem::path Build(AssetHandle textureHandle, const Region& srcRegion);
			static std::filesystem::path Build(AssetHandle textureHandle);
		};

		Sprite(AssetHandle textureHandle, const Region& srcRegion);
		Sprite(AssetHandle textureHandle);

		Ref<Texture2D> GetTexture() const;
		const Region& GetSrcRegion() const;

		static AssetType GetStaticType() { return AssetType::Sprite; };
		virtual AssetType GetType() const { return GetStaticType(); };
	private:
		AssetHandle m_TextureHandle{ AssetHandle::InvalidUUID };
		Ref<Texture2D> m_pTexture{};
		Region m_SrcRegion{};
	};
}