#pragma once
#include <string>
#include <memory>
#include "JREngine/Rendering/RendererComponentBase.h"
#include "JREngine/Asset/Asset.h"

namespace JRE
{
	class Font;
	class Sprite;
	class TextRendererComponent final : public RendererComponentBase
	{
	public:
		TextRendererComponent(GameObject& gameObject, const std::string& text, AssetHandle fontHandle);

		TextRendererComponent(const TextRendererComponent& other) = delete;
		TextRendererComponent(TextRendererComponent&& other) = delete;
		TextRendererComponent& operator=(const TextRendererComponent& other) = delete;
		TextRendererComponent& operator=(TextRendererComponent&& other) = delete;

		virtual void Update() override;
		virtual void Render() const override;

		void SetText(const std::string& text);
	private:
		bool m_NeedsUpdate{};
		std::string m_Text{};
		AssetHandle m_FontHandle{ AssetHandle::InvalidUUID };
		AssetHandle m_SpriteHandle{ AssetHandle::InvalidUUID };
		Ref<Sprite> m_pSprite{};
	};
}
