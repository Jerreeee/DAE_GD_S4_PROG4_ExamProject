#pragma once
#include <string>
#include <memory>
#include "ComponentBase.h"
#include "TransformComponent.h"

namespace dae
{
	class Font;
	class Texture2D;
	class TextRendererComponent final : public ComponentBase
	{
	public:
		TextRendererComponent(GameObject& gameObject, const std::string& text, std::shared_ptr<Font> font);
		TextRendererComponent(const TextRendererComponent& other) = delete;
		TextRendererComponent(TextRendererComponent&& other) = delete;
		TextRendererComponent& operator=(const TextRendererComponent& other) = delete;
		TextRendererComponent& operator=(TextRendererComponent&& other) = delete;

		void Update() override;
		void Render() const override;

		void SetText(const std::string& text);
		void SetPosition(float x, float y);
	private:
		bool m_NeedsUpdate;
		std::string m_Text;
		TransformComponent m_Transform{};
		std::shared_ptr<Font> m_Font;
		std::shared_ptr<Texture2D> m_TextTexture;
	};
}
