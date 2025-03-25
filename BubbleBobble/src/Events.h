#pragma once
#include <string_view>
#include "JREngine/Event.h"

namespace BubbleBobble
{
	struct Event
	{
		struct PlayerDamaged
		{
			static const JREngine::EventID ID{ JREngine::HashEventID("PlayerDamaged") };
			struct Args : public JREngine::EventArgs
			{
				Args(int _damage, int _newHealth)
					: damage{ _damage }, newhealth{ _newHealth }
				{
				}
				int damage;
				int newhealth;
			};
		};

		struct IncreasedScore
		{
			static const JREngine::EventID ID{ JREngine::HashEventID("IncreasedScore") };
			struct Args : public JREngine::EventArgs
			{
				Args(int _newScore) : newScore{ _newScore } {};
				int newScore;
			};
		};
	};
}
