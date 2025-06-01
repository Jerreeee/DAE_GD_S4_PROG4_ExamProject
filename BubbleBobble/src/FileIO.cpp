#include <fstream>
#include <sstream>
#include <stdexcept>
#include "FileIO.h"

namespace BubbleBobble::FileIO
{
	std::vector<AnimData> GetAnimData(const std::filesystem::path& path)
	{
        std::ifstream fStream(path.string().c_str());
        if (!fStream)
            throw std::runtime_error("Failed to open animation file: " + path.string());

        std::vector<AnimData> dataVec{};

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
                //Trim quotes "" if present
                if (!token.empty() && token.front() == '"' && token.back() == '"')
                    token = token.substr(1, token.size() - 2);
                tokens.emplace_back(token);
            }

            if (tokens.size() != 7)
                throw std::runtime_error("Incorrectly formed animation data: " + line);

            AnimData data{};
            data.path = tokens[0];
            data.frameCount = std::stoi(tokens[1]);
            data.cols = std::stoi(tokens[2]);
            data.rows = std::stoi(tokens[3]);
            data.fps = std::stoi(tokens[4]);
            data.isPong = (tokens[5] == "true");
            data.animName = tokens[6];

            dataVec.emplace_back(data);
        }
        return dataVec;
	}
}
