#pragma once
#include <string_view>
#include "JREngine/Core/Event.h"

namespace BubbleBobble
{
	struct Event
	{

		struct IncreasedScore
		{
			static const JRE::EventID ID{ JRE::HashEventID("IncreasedScore") };
			struct Args : public JRE::EventArgs
			{
				Args(int _newScore) : newScore{ _newScore } {};
				int newScore;
			};
		};
	};
}
