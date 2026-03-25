#include <iomanip>
#include <sstream>
#include <fstream>
#include "Asset/AssetRegistry.h"
#include "JREngine/Core/UUID.h"

namespace JRE
{
	AssetHandle AssetRegistry::Register(AssetMetadata metadata)
	{
		std::string virtualPath = metadata.GetVirtualPath();

		// Dedup check
		auto it = m_PathToAssetHandle.find(virtualPath);
		if (it != m_PathToAssetHandle.end())
			return it->second;

		// Hash-based handle, same virtual path always gives the same handle
		AssetHandle handle = UUID::FromPath(virtualPath);
		std::lock_guard<std::mutex> lock(m_Mutex);
		m_AssetHandleToMetadata.emplace(handle, std::move(metadata));
		m_PathToAssetHandle.emplace(virtualPath, handle);
		return handle;
	}

	bool AssetRegistry::IsValidAssetHandle(AssetHandle handle) const
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		return m_AssetHandleToMetadata.find(handle) != m_AssetHandleToMetadata.end();
	}

	bool AssetRegistry::HasAssetAtPath(const std::string& virtualPath) const
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		return m_PathToAssetHandle.find(virtualPath) != m_PathToAssetHandle.end();
	}

	AssetHandle AssetRegistry::GetHandleAtPath(const std::string& virtualPath) const
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		auto it = m_PathToAssetHandle.find(virtualPath);
		return it != m_PathToAssetHandle.end() ? it->second : AssetHandle::InvalidUUID;
	}

	const AssetMetadata& AssetRegistry::GetMetadata(AssetHandle handle) const
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		static AssetMetadata nullMetadata;
		auto it = m_AssetHandleToMetadata.find(handle);
		return it != m_AssetHandleToMetadata.end() ? it->second : nullMetadata;
	}

	void AssetRegistry::Serialize(std::ostream& out) const
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		out << "JRMA 1\n";
		out << "count " << m_AssetHandleToMetadata.size() << "\n\n";

		for (auto& [handle, meta] : m_AssetHandleToMetadata)
		{
			out << std::hex << std::setw(16) << std::setfill('0')
				<< static_cast<uint64_t>(handle) << std::dec;
			out << ',' << meta.assetType;
			out << ',' << meta.filepath.generic_string();
			out << ',' << meta.uniqueID;
			out << ',' << (meta.canLoadAsync ? 1 : 0);
			out << ',' << meta.dependencies.size();
			for (AssetHandle dep : meta.dependencies)
				out << ',' << std::hex << std::setw(16) << std::setfill('0')
					<< static_cast<uint64_t>(dep) << std::dec;
			out << '\n';
		}
	}

	void AssetRegistry::SetDataPath(const std::filesystem::path& dataPath)
	{
		m_DataPath = dataPath;
	}

	const std::filesystem::path& AssetRegistry::GetDatapath() const
	{
		return m_DataPath;
	}

	std::filesystem::path AssetRegistry::GetFullDatapath(const std::filesystem::path& relativePath) const
	{
		return m_DataPath / relativePath;
	}

	bool AssetRegistry::Deserialize(std::istream& in)
	{
		std::string header;
		if (!std::getline(in, header) || header != "JRMA 1")
			return false;

		std::string countLine;
		if (!std::getline(in, countLine) || countLine.rfind("count ", 0) != 0)
			return false;

		std::string line;
		while (std::getline(in, line))
		{
			if (line.empty() || line[0] == '/') continue;

			std::vector<std::string> tok;
			std::stringstream ss(line);
			std::string t;
			while (std::getline(ss, t, ',')) tok.push_back(t);
			if (tok.size() < 6) continue;

			AssetHandle handle{ std::stoull(tok[0], nullptr, 16) };
			AssetMetadata meta;
			meta.assetType    = tok[1];
			meta.filepath     = tok[2];
			meta.uniqueID     = tok[3];
			meta.canLoadAsync = tok[4] == "1";
			uint32_t depCount = static_cast<uint32_t>(std::stoul(tok[5]));
			for (uint32_t d = 0; d < depCount; ++d)
				meta.dependencies.emplace_back(std::stoull(tok[6 + d], nullptr, 16));

			std::string vPath = meta.GetVirtualPath();
			if (m_PathToAssetHandle.find(vPath) == m_PathToAssetHandle.end())
			{
				m_AssetHandleToMetadata.emplace(handle, meta);
				m_PathToAssetHandle.emplace(vPath, handle);
			}
		}
		return in.eof() || in.good();
	}
}
