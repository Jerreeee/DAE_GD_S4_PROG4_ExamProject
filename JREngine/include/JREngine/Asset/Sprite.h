#pragma once
#include <memory>
#include "JREngine/Asset/SoftAssetRef.h"
#include "JREngine/Math/Rect.h"
#include "JREngine/Asset/Texture2D.h"

namespace JRE
{
	class Texture2D;
	class Sprite final : public Asset
	{
	public:
		Sprite(SoftAssetRef<Texture2D> softTextureRef, const Region& srcRegion);
		Sprite(SoftAssetRef<Texture2D> softTextureRef);

		AssetRef<Texture2D> GetTexture() const;
		const Region& GetSrcRegion() const;

		static AssetType GetStaticType() { return AssetType::Sprite; };
		virtual AssetType GetType() const { return GetStaticType(); };
	private:
		void Initialize() const;

		SoftAssetRef<Texture2D> m_SoftTextureRef{};
		bool m_UsesWholeTextureRegion{};
		mutable Region m_SrcRegion{};
		mutable bool m_IsInitialized{ false };
	};
}
