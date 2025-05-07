#pragma once
#include <vector>
#include "JREngine/Resources/Asset.h"

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
		static std::vector<AssetHandle> SplitTexture2D(AssetHandle textureHandle, int nrSprites, int cols, int rows);
	};
}