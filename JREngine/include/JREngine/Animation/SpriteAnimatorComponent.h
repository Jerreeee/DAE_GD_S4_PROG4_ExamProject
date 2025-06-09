#pragma once
#include <map>
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

		void AddClip(const std::string& name, AssetRef<SpriteAnimationClip> clip, bool setActive = false);
		void SetActiveClip(const std::string& name);
	private:
		SpriteRendererComponent* m_pSpriteRendererComponent{ nullptr };
		AssetRef<SpriteAnimationClip>* m_pActiveClip{ nullptr };
		std::map<std::string, AssetRef<SpriteAnimationClip>> m_Clips{};
	};
}
