#pragma once
#include <cstdint>
#include <memory>
#include <utility>
#include "JREngine/Core/UUID.h"

namespace JRE
{
	enum class AssetType
	{
		None,
		Texture2D,
		Font,
		SoundClip,
		Sprite
	};

	using AssetHandle = UUID;

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename... Args>
	Ref<T> CreateRef(Args&&... args)
	{
		return Ref<T>(std::forward<Args>(args)...);
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
		AssetHandle m_Handle;
	};
}