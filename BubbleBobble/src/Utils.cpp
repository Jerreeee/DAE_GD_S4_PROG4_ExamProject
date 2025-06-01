#include "JREngine/Asset/ResourceManager.h"
#include "JREngine/Asset/SpriteEditor.h"
#include "JREngine/Asset/AssetImporter.h"
#include "JREngine/Asset/TextureImporter.h"
#include "Utils.h"

namespace BubbleBobble::Utils
{
	JRE::AssetRef<JRE::SpriteAnimationClip> CreateAnimationClipFromAnimData(const AnimData& data)
	{
		auto logoHandle = JRE::AssetImporter::GetInstance().ImportAsset(JRE::TextureImporter(data.path));
		auto logoRef = JRE::ResourceManager::GetAsset<JRE::Texture2D>(logoHandle);
		auto spritesRef = JRE::SpriteEditor::SplitTexture2D(logoRef, data.frameCount, data.cols, data.rows);
		std::vector<JRE::SoftAssetRef<JRE::Sprite>> spriteSoftRefs{};
		for (auto& spriteRef : spritesRef)
			spriteSoftRefs.emplace_back(JRE::SoftAssetRef<JRE::Sprite>(spriteRef));
		return JRE::CreateAssetRef<JRE::SpriteAnimationClip>(spriteSoftRefs, data.fps);
	}
}
