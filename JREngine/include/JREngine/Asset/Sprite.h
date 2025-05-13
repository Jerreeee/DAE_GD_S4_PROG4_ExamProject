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
		Sprite(const SoftAssetRef<Texture2D>& softTextureRef, const Region& srcRegion);
		Sprite(const SoftAssetRef<Texture2D>& softTextureRef);

		AssetRef<Texture2D> GetTexture() const;
		const Region& GetSrcRegion() const;

		static constexpr std::string_view GetStaticType() { return "Sprite"; };
		virtual std::string_view GetType() const { return GetStaticType(); };
	private:
		void Initialize() const;

		SoftAssetRef<Texture2D> m_SoftTextureRef{};
		bool m_UsesWholeTextureRegion{};
		mutable Region m_SrcRegion{};
		mutable bool m_IsInitialized{ false };
	};
}
