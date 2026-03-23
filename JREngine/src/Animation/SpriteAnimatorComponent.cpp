#include "JREngine/Scene/GameObject.h"
#include "JREngine/Rendering/SpriteRendererComponent.h"
#include "JREngine/Asset/ResourceManager.h"
#include "JREngine/Asset/Sprite.h"
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
		if (!m_pActiveClip)
			return;

		(*m_pActiveClip)->Update();
		if (m_pSpriteRendererComponent)
		{
			auto sprite = ResourceManager::GetAsset<Sprite>((*m_pActiveClip)->GetCurrentSpriteHandle());
			m_pSpriteRendererComponent->SetSprite(sprite);
		}
	}
	void SpriteAnimatorComponent::AddClip(const std::string& name, AssetRef<SpriteAnimationClip> clip, bool setActive)
	{
		m_Clips.emplace(name, clip);
		if (setActive)
			SetActiveClip(name);
	}
	void SpriteAnimatorComponent::SetActiveClip(const std::string& name)
	{
		auto it = m_Clips.find(name);
		m_pActiveClip = it != m_Clips.end() ? &it->second : nullptr;
	}
	AssetRef<SpriteAnimationClip> SpriteAnimatorComponent::GetClip(const std::string& name)
	{
		auto it = m_Clips.find(name);
		return it != m_Clips.end() ? it->second : nullptr;
	}
}
