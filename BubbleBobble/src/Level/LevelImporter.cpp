#include "JREngine/Asset/AssetImporter.h"
#include "Level/Level.h"
#include "Level/LevelImporter.h"

namespace BubbleBobble
{
	static bool s_Registered = []()
		{
			JRE::AssetImporter::GetInstance().RegisterImporter(Level::GetStaticType(), LevelImporter::ImportAsset);
			return true;
		}();

	LevelImporter::LevelImporter(const std::filesystem::path& filepath) :
		m_Path{ JRE::AssetImporter::GetInstance().GetFullDatapath(filepath) }
	{
	}
	JRE::AssetRef<JRE::Asset> LevelImporter::ImportAsset(JRE::AssetHandle, const JRE::AssetMetadata&)
	{
		return nullptr;
	}
	JRE::AssetMetadata LevelImporter::GetMetadata() const
	{
		return JRE::AssetMetadata{ Level::GetStaticType().data(), m_Path, "", true };
	}
}
