#include "JREngine/Scene/GameObject.h"
#include "JREngine/Animation/SpriteAnimationClip.h"
#include "JREngine/Rendering/SpriteRendererComponent.h"
#include "JREngine/Animation/SpriteAnimatorComponent.h"

#include "Enemies/Zenchan/ZenchanBuilder.h"

using namespace JRE;

namespace BubbleBobble
{
	void ZenchanBuilder::Build(std::unique_ptr<JRE::GameObject>& pZenchan)
	{
        pZenchan->m_Persistant = true;
        pZenchan->AddComponent<JRE::SpriteRendererComponent>();
        pZenchan->AddComponent<JRE::SpriteAnimatorComponent>();
        //SetAnimations(pScriptCmp);
	}
}
