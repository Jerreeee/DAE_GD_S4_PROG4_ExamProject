#pragma once
#include <vector>
#include "JREngine/Asset/Asset.h"

namespace JRE
{
	class SpriteEditor final
	{
	public:
		/// <summary>
		/// Splits the provided texture into multiple sprites.
		/// Adds all the generated sprites to the ResourceManager
		/// </summary>
		/// <returns> AssetHandles to the created sprites </returns>
		static std::vector<AssetRef<Sprite>> SplitTexture2D(AssetRef<Texture2D> textureRef, int nrSprites, int cols, int rows);
	};
}