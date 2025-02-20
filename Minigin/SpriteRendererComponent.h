#pragma once
#include "ComponentBase.h"
#include <string>

namespace dae
{
	class Texture2D;
	class SpriteRendererComponent final: public ComponentBase
	{
	public:
		SpriteRendererComponent(GameObject& gameObject);

		SpriteRendererComponent(const SpriteRendererComponent& other) = delete;
		SpriteRendererComponent(SpriteRendererComponent&& other) = delete;
		SpriteRendererComponent& operator=(const SpriteRendererComponent& other) = delete;
		SpriteRendererComponent& operator=(SpriteRendererComponent&& other) = delete;

		virtual void FixedUpdate() override {};
		virtual void Update() override {};
		virtual void Render() const override;

		void SetTexture(const std::string& filename);
	private:
		std::shared_ptr<Texture2D> m_Texture{};
	};
}

