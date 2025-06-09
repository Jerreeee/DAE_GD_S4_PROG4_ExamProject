#pragma once
#include "JREngine/Asset/Asset.h"
#include "JREngine/Animation/SpriteAnimationClip.h"

namespace JRE
{
	class SpriteAnimatorComponent;
}

namespace BubbleBobble
{
	struct AnimData;
}

namespace BubbleBobble::Utils
{
	JRE::AssetRef<JRE::SpriteAnimationClip> CreateAnimationClipFromAnimData(const AnimData& data);

	void AddAnimsToSpriteAnimatorComponent(const std::filesystem::path& animPath, JRE::SpriteAnimatorComponent& comp);
}
