#pragma once
#include <vector>
#include "JREngine/Asset/Asset.h"

namespace JRE
{
	class Sprite;
	class SpriteEditor final
	{
	public:
		//Splits the provided texture into multiple sprites.
		static std::vector<AssetRef<Sprite>> SplitTexture2D(AssetRef<Texture2D> textureRef, int nrSprites, int cols, int rows);
	};
}