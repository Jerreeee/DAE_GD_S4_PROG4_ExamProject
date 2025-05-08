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
		Sprite(AssetHandle textureHandle, const Region& srcRegion);
		Sprite(AssetHandle textureHandle);

		Ref<Texture2D> GetTexture() const;
		const Region& GetSrcRegion() const;

		static AssetType GetStaticType() { return AssetType::Sprite; };
		virtual AssetType GetType() const { return GetStaticType(); };
	private:
		void ForceLoad() const;

		AssetHandle m_TextureHandle{};
		mutable Ref<Texture2D> m_pTexture{};
		bool m_UsesWholeTextureRegion;
		mutable Region m_SrcRegion;
	};
}
