#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cassert>
#include "JREngine/Asset/AssetImporter.h"
#include "Assets/AnimsDataImporter.h"

using namespace JRE;

namespace BubbleBobble
{
	static bool s_Registered = []()
		{
			AssetImporter::GetInstance().RegisterImporter(AnimsData::GetStaticType(), AnimDataImporter::ImportAsset);
			return true;
		}();

	AnimDataImporter::AnimDataImporter(const std::filesystem::path& filepath) :
		m_Path{ AssetImporter::GetInstance().GetFullDatapath(filepath) }
	{
	}

	AssetRef<Asset> AnimDataImporter::ImportAsset(AssetHandle, const AssetMetadata& metadata)
	{
        std::ifstream fStream(metadata.filepath.string().c_str());
        if (!fStream)
            throw std::runtime_error("Failed to open animation file: " + metadata.filepath.string());

        auto animsDataRef = CreateAssetRef<AnimsData>();

        std::string line{};
        std::getline(fStream, line);
        while (std::getline(fStream, line))
        {
            if (line.empty() || line[0] == '/')
                continue;

            std::stringstream ss(line);
            std::string token{};
            std::vector<std::string> tokens{};

            while (std::getline(ss, token, ','))
            {
                if (!token.empty() && token.front() == '"' && token.back() == '"')
                    token = token.substr(1, token.size() - 2);
                tokens.emplace_back(token);
            }
            assert(tokens.size() == 7 && "Ill-formed animData");

            AnimData data{};
            data.path = tokens[0];
            data.frameCount = std::stoi(tokens[1]);
            data.cols = std::stoi(tokens[2]);
            data.rows = std::stoi(tokens[3]);
            data.fps = std::stoi(tokens[4]);
            data.isPong = (tokens[5] == "true");
            data.animName = tokens[6];

            animsDataRef->dataVec.emplace_back(data);
        }
        return animsDataRef;
	}

	AssetMetadata AnimDataImporter::GetMetadata() const
	{
		return AssetMetadata{ AnimsData::GetStaticType().data(), m_Path, "", false };
	}
}
