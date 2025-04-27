#pragma once
#include <string>
#include <memory>
#include "RendererComponentBase.h"
#include "ResourceHandle.h"

namespace JRE
{
	class Font;
	class Texture2D;
	class TextRendererComponent final : public RendererComponentBase
	{
	public:
		TextRendererComponent(GameObject& gameObject, const std::string& text, ResourceHandle<Font> fontHandle);

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
		ResourceHandle<Font> m_FontHandle{};
		std::shared_ptr<Font> m_pFont{};
		ResourceHandle<Texture2D> m_TextTextureHandle{};
		std::shared_ptr<Texture2D> m_pTextTexture{};
	};
}
