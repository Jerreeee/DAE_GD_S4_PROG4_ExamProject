#pragma once
#include <memory>
#include <filesystem> //all deriving classes and AssetImporters will probably need this
#include "JREngine/Core/UUID.h"

namespace JRE
{
	class Asset;
}

template<typename T>
concept IsAsset = std::derived_from<T, JRE::Asset>;

namespace JRE
{
	enum class AssetType
	{
		None,
		Texture2D,
		Font,
		SoundClip,
		Sprite,
		SpriteAnimationClip
	};

	enum class AssetLoadMode
	{
		Unspecified,
		Immediate,
		Async
	};

	using AssetHandle = UUID;

	template<typename T>
	using AssetRef = std::shared_ptr<T>;

	template<IsAsset T, typename... Args>
	AssetRef<T> CreateAssetRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	class Asset
	{
	public:
		Asset() = default;
		explicit Asset(AssetHandle handle) : m_Handle(handle) {};

		void SetHandle(AssetHandle handle) { m_Handle = handle; }
		AssetHandle GetHandle() const { return m_Handle; }
		virtual AssetType GetType() const = 0;
	private:
		//Invalid handle by default so for any class inheriting from Asset
		//an instance can be created without generating a new valid UUID.
		//This is nice if you want to manually manage an asset instance
		//without relying on an AssetManager. And it is only such an AssetManager
		//that would need a valid UUID to map from AssetHandle -> Asset
		AssetHandle m_Handle{ AssetHandle::InvalidUUID };
	};
}
