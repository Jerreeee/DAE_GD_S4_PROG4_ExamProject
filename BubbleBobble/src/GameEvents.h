#pragma once
#include <string_view>
#include "JREngine/Event.h"

namespace Game
{
	struct Event
	{
		struct PlayerDamaged
		{
			static const Engine::EventID ID{ Engine::HashEventID("PlayerDamaged") };
			struct Args : public Engine::EventArgs
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
			static const Engine::EventID ID{ Engine::HashEventID("IncreasedScore") };
			struct Args : public Engine::EventArgs
			{
				Args(int _newScore) : newScore{ _newScore } {};
				int newScore;
			};
		};
	};
}
