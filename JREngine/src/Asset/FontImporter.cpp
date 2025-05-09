#include "Asset/Font.h"
#include "Asset/AssetImporter.h"
#include "Asset/FontImporter.h"

namespace JRE
{
	static bool s_Registered = []()
		{
			AssetImporter::GetInstance().RegisterImporter(AssetType::Font, FontImporter::ImportAsset);
			return true;
		}();

	FontImporter::FontImporter(const std::filesystem::path& filepath) :
		m_Path{ AssetImporter::GetInstance().GetFullDatapath(filepath) }
	{
	}

	Ref<Asset> FontImporter::ImportAsset(AssetHandle, const AssetMetadata& metadata)
	{
		const std::string& id = metadata.uniqueID;

		if (id.empty() || id[0] != '@')
			throw std::runtime_error("Invalid Font metadata.uniqueID format");

		uint8_t size = static_cast<uint8_t>(std::stoi(id.substr(1)));
		return CreateRef<Font>(metadata.filepath, size);
	}

	AssetMetadata FontImporter::GetMetadata() const
	{
		return AssetMetadata{ AssetType::Font, m_Path, GetUniqueID(), true };
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
