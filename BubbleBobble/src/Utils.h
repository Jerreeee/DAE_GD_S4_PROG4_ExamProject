#pragma once
#include "JREngine/Asset/Asset.h"
#include "JREngine/Animation/SpriteAnimationClip.h"
#include "FileIO.h"

namespace BubbleBobble::Utils
{
	JRE::AssetRef<JRE::SpriteAnimationClip> CreateAnimationClipFromAnimData(const AnimData& data);
}
