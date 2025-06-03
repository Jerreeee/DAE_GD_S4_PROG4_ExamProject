#pragma once
#include "JREngine/Asset/Asset.h"
#include "JREngine/Animation/SpriteAnimationClip.h"

namespace BubbleBobble
{
	struct AnimData;
}

namespace BubbleBobble::Utils
{
	JRE::AssetRef<JRE::SpriteAnimationClip> CreateAnimationClipFromAnimData(const AnimData& data);
}
