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
		std::vector<AnimData> dataVec;

		static constexpr std::string_view GetStaticType() { return "BubbleBobble::AnimsData"; };
		virtual std::string_view GetType() const { return GetStaticType(); };
	};
}
