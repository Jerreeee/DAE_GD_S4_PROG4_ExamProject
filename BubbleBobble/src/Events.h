#pragma once
#include <string_view>
#include "JREngine/Core/Event.h"

namespace BubbleBobble
{
	struct Event
	{
		struct PlayerDamaged
		{
			static const JRE::EventID ID{ JRE::HashEventID("PlayerDamaged") };
			struct Args : public JRE::EventArgs
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
			static const JRE::EventID ID{ JRE::HashEventID("IncreasedScore") };
			struct Args : public JRE::EventArgs
			{
				Args(int _newScore) : newScore{ _newScore } {};
				int newScore;
			};
		};
	};
}
