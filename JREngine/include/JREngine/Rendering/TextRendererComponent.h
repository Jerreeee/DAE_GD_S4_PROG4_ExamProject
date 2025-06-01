#pragma once
#include <string>
#include <memory>
#include "JREngine/Rendering/RendererComponentBase.h"
#include "JREngine/Asset/Asset.h"
#include "JREngine/Asset/SoftAssetRef.h"
#include "JREngine/Asset/Font.h"

namespace JRE
{
	class Font;
	class Sprite;
	class TextRendererComponent final : public RendererComponentBase
	{
	public:
		TextRendererComponent(GameObject& gameObject, const std::string& text, const SoftAssetRef<Font>& softFontRef);

		TextRendererComponent(const TextRendererComponent& other) = delete;
		TextRendererComponent(TextRendererComponent&& other) = delete;
		TextRendererComponent& operator=(const TextRendererComponent& other) = delete;
		TextRendererComponent& operator=(TextRendererComponent&& other) = delete;

		virtual void Update() override;
		virtual void Render() const override;

		void SetText(const std::string& text);
		const Sprite& GetSprite() const;
	private:
		void UpdateTexture() const;

		mutable bool m_NeedsUpdate{};
		std::string m_Text{};
		SoftAssetRef<Font> m_SoftFontRef{};
		mutable AssetRef<Sprite> m_Sprite{ nullptr };
	};
}
