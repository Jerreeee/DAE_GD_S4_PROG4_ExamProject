#pragma once
#include <filesystem>
#include "JREngine/Core/Singleton.h"
#include "JREngine/Asset/IAssetImporter.h"
#include "JREngine/Asset/AssetMetadata.h"

namespace JRE
{
	/// Editor-phase singleton. Knows about IAssetImporter.
	/// Responsible for: populating AssetRegistry, resolving data paths, writing the manifest.
	/// Has no knowledge of how assets are loaded at runtime — that is AssetLoaderRegistry's job.
	class AssetDatabase : public Singleton<AssetDatabase>
	{
	public:
		void Init(const std::filesystem::path& dataPath);

		// Register an asset in AssetRegistry. Called during editor/registration phase only.
		AssetHandle RegisterAsset(IAssetImporter&& importer);

		// Write the manifest file. Called once after all assets are registered.
		void SerializeManifest(const std::filesystem::path& manifestPath) const;

		const std::filesystem::path& GetDatapath() const;
		std::filesystem::path GetFullDatapath(const std::filesystem::path& filepath) const;

	private:
		AssetDatabase() = default;
		friend class Singleton<AssetDatabase>;

		std::filesystem::path m_Datapath{};
	};
}
