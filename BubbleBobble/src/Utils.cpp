#include "JREngine/Asset/ResourceManager.h"
#include "JREngine/Asset/Sprite.h"
#include "JREngine/Asset/SpriteEditor.h"
#include "JREngine/Animation/SpriteAnimatorComponent.h"

#include "EngineSetup.h"
#include "Assets/AnimsData.h"
#include "Utils.h"

using namespace JRE;

namespace BubbleBobble::Utils
{
	JRE::AssetRef<JRE::SpriteAnimationClip> CreateAnimationClipFromAnimData(const AnimData& data)
	{
		auto handle = GetRegisteredHandle(data.path);
		auto textureRef = ResourceManager::GetAsset<Texture2D>(handle);
		auto spritesRef = SpriteEditor::SplitTexture2D(textureRef, data.frameCount, data.cols, data.rows);
		std::vector<AssetHandle> spriteHandles{};
		for (auto& spriteRef : spritesRef)
			spriteHandles.emplace_back(ResourceManager::AddAsset(spriteRef));
		return JRE::CreateAssetRef<SpriteAnimationClip>(spriteHandles, data.fps, data.isPong);
	}
	void AddAnimsToSpriteAnimatorComponent(const std::filesystem::path& animPath, JRE::SpriteAnimatorComponent& comp)
	{
		AssetHandle animsDataHandle = GetRegisteredHandle(animPath.generic_string());
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
