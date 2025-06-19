#pragma once
#include <string>
#include "JREngine/Asset/Asset.h"

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

	struct AnimsData : public JRE::Asset
	{
		REGISTER_TYPE_HEADER(JRE::Asset);

		std::vector<AnimData> dataVec;
	};
}
