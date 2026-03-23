#include "Asset/Font.h"
#include "Asset/AssetLoaderRegistry.h"
#include "Asset/AssetRegistry.h"
#include "Asset/FontImporter.h"

namespace JRE
{
	static bool s_Registered = []()
		{
			AssetLoaderRegistry::GetInstance().RegisterLoader(Font::GetStaticType(), FontImporter::Load);
			return true;
		}();

	FontImporter::FontImporter(const std::filesystem::path& filepath) :
		m_Path{ filepath }
	{
	}

	AssetRef<Asset> FontImporter::Load(AssetHandle, const AssetMetadata& metadata)
	{
		const std::string& id = metadata.uniqueID;

		if (id.empty() || id[0] != '@')
			throw std::runtime_error("Invalid Font metadata.uniqueID format");

		uint8_t size = static_cast<uint8_t>(std::stoi(id.substr(1)));
		auto fullPath = AssetRegistry::GetInstance().GetFullDatapath(metadata.filepath);
		return CreateAssetRef<Font>(fullPath, size);
	}

	AssetMetadata FontImporter::GetMetadata() const
	{
		return AssetMetadata{ Font::GetStaticType().data(), m_Path, GetUniqueID(), true, GetDeclaredDependencies() };
	}

	std::string FontImporter::GetUniqueID() const
	{
		std::stringstream ss{};
		ss << '@' << std::to_string(m_Size);
		return ss.str();
	}

	FontImporter& FontImporter::SetSize(int size)
	{
		m_Size = size;
		return *this;
	}
}
