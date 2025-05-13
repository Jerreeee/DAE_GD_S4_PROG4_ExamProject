#include "JREngine/Scene/GameObject.h"
#include "JREngine/Rendering/SpriteRendererComponent.h"
#include "Animation/SpriteAnimatorComponent.h"

namespace JRE
{
	SpriteAnimatorComponent::SpriteAnimatorComponent(GameObject& gameObject) :
		ComponentBase(gameObject)
	{
		auto& obj = GetGameObject();
		m_pSpriteRendererComponent = obj.GetComponent<SpriteRendererComponent>();
	}
	void SpriteAnimatorComponent::Update()
	{
		if (!m_SpriteAnimationclip) return;

		m_SpriteAnimationclip->Update();
		if (m_pSpriteRendererComponent)
			m_pSpriteRendererComponent->SetSprite(m_SpriteAnimationclip->GetCurrentSprite());
	}
	void SpriteAnimatorComponent::SetSpriteAnimationClip(AssetRef<SpriteAnimationClip> clip)
	{
		m_SpriteAnimationclip = clip;
	}
}
