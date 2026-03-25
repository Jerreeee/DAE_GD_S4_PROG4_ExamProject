#pragma once
#include <filesystem>
#include "JREngine/Core/Singleton.h"
#include "JREngine/Asset/IAssetImporter.h"

namespace JRE
{
	/// Editor-phase singleton. Knows about IAssetImporter.
	/// Responsible for: populating AssetRegistry and writing the manifest.
	/// Has no knowledge of how assets are loaded at runtime — that is AssetLoaderRegistry's job.
	class AssetDatabase : public Singleton<AssetDatabase>
	{
	public:
		void Init(const std::filesystem::path& dataPath);

		// Register an asset in AssetRegistry. Called during editor/registration phase only.
		AssetHandle RegisterAsset(IAssetImporter&& importer);

		// Write the manifest file. Called once after all assets are registered.
		void SerializeManifest(const std::filesystem::path& manifestPath) const;

	private:
		AssetDatabase() = default;
		friend class Singleton<AssetDatabase>;
	};
}
