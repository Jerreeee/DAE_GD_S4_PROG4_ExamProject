#pragma once
#include "ComponentBase.h"
#include <string>

namespace dae
{
	class Texture2D;
	class SpriteRendererComponent : public ComponentBase
	{
	public:
		SpriteRendererComponent(GameObject& gameObject);

		virtual void Render() const override;

		void SetTexture(const std::string& filename);
	private:
		std::shared_ptr<Texture2D> m_Texture{};
	};
}

