#pragma once
#include "JREngine/Scene/ComponentBase.h"
#include "JREngine/Asset/Asset.h"
#include "JREngine/Animation/SpriteAnimationClip.h"

namespace JRE
{
	class SpriteRendererComponent;
	class SpriteAnimatorComponent : public ComponentBase
	{
	public:
		SpriteAnimatorComponent(GameObject& gameObject);

		virtual void Update() override;

		void SetSpriteAnimationClip(AssetRef<SpriteAnimationClip> clip);
	private:
		SpriteRendererComponent* m_pSpriteRendererComponent{ nullptr };
		AssetRef<SpriteAnimationClip> m_SpriteAnimationclip{ nullptr };
	};
}
