#pragma once
#include <string>
#include <filesystem>
#include <vector>

namespace BubbleBobble
{
	struct AnimData
	{
		std::string path;
		int frameCount;
		int cols;
		int rows;
		int fps;
		bool isPong;
		std::string animName;
	};
}

namespace BubbleBobble::FileIO
{
	std::vector<AnimData> GetAnimData(const std::filesystem::path& path);
}
