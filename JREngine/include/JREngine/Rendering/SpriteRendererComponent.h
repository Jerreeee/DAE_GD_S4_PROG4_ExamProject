#pragma once
#include <string>
#include "JREngine/Asset/Asset.h"
#include "JREngine/Rendering/RendererComponentBase.h"

namespace JRE
{
	class Sprite;
	class SpriteRendererComponent final: public RendererComponentBase
	{
	public:
		SpriteRendererComponent(GameObject& gameObject);

		SpriteRendererComponent(const SpriteRendererComponent& other) = delete;
		SpriteRendererComponent(SpriteRendererComponent&& other) = delete;
		SpriteRendererComponent& operator=(const SpriteRendererComponent& other) = delete;
		SpriteRendererComponent& operator=(SpriteRendererComponent&& other) = delete;

		virtual void Render() const override;

		void SetSprite(const AssetRef<Sprite>& sprite);
		void SetFlipX(bool flipX) { m_FlipX = flipX; };
	private:
		AssetRef<Sprite> m_Sprite{};
		bool m_FlipX{};
	};
}

