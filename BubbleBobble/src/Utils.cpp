#include "JREngine/Asset/ResourceManager.h"
#include "JREngine/Asset/SpriteEditor.h"
#include "JREngine/Asset/AssetImporter.h"
#include "JREngine/Asset/TextureImporter.h"
#include "JREngine/Animation/SpriteAnimatorComponent.h"

#include "EngineSetup.h"
#include "Assets/AnimsDataImporter.h"
#include "Utils.h"

using namespace JRE;

namespace BubbleBobble::Utils
{
	JRE::AssetRef<JRE::SpriteAnimationClip> CreateAnimationClipFromAnimData(const AnimData& data)
	{
		auto handle = AssetImporterRegistry::GetInstance().ImportAsset(TextureImporter(data.path));
		auto textureRef = ResourceManager::GetAsset<Texture2D>(handle);
		auto spritesRef = SpriteEditor::SplitTexture2D(textureRef, data.frameCount, data.cols, data.rows);
		std::vector<SoftAssetRef<Sprite>> spriteSoftRefs{};
		for (auto& spriteRef : spritesRef)
			spriteSoftRefs.emplace_back(SoftAssetRef<Sprite>(spriteRef));
		return JRE::CreateAssetRef<SpriteAnimationClip>(spriteSoftRefs, data.fps, data.isPong);
	}
	void AddAnimsToSpriteAnimatorComponent(const std::filesystem::path& animPath, JRE::SpriteAnimatorComponent& comp)
	{
		auto animsDataImporter = AnimDataImporter(animPath);
		AssetHandle animsDataHandle = AssetImporterRegistry::GetInstance().ImportAsset(std::move(animsDataImporter));
		AssetRef<AnimsData> animsDataRef = ResourceManager::GetAsset<AnimsData>(animsDataHandle);

		for (const AnimData& animData : animsDataRef->dataVec)
		{
			auto clipRef = CreateAnimationClipFromAnimData(animData);
			comp.AddClip(animData.animName, clipRef);
		}
	}
	JRE::CollisionDir PlatformCollisionDirFilterFunc(const StaticCollider& collider, const JRE::BoxPhysicsSystem::CollisionSettings& cs)
	{
		JRE::CollisionDir collDir{};

		const BoxShape& colliderBox = static_cast<const BoxShape&>(*collider.shape);
		const BoxShape& box = static_cast<const BoxShape&>(cs.collider.GetShape());
		BoxShape worldBox = box.Translated(cs.oldPos);
		bool isPlatform = collider.properties.layer & CollisionLayer::Platform;
		bool alreadyCollidingY = worldBox.OverlapInY(colliderBox);
		bool skipDownColl = isPlatform && alreadyCollidingY;

		collDir.up = !isPlatform;
		collDir.down = !skipDownColl;
		collDir.left = !isPlatform;
		collDir.right = !isPlatform;
		return collDir;
	}
}
