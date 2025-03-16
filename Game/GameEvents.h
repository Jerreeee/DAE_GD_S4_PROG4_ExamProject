#pragma once
#include "../Minigin/Event.h"

namespace Game
{
	struct DamagedEventArgs : public Engine::EventArgs
	{
		DamagedEventArgs(int _damage, int _newHealth)
			: damage{ _damage }, newhealth{ _newHealth }
		{}
		int damage;
		int newhealth;
	};

	struct IncreasedScoreEventArgs : public Engine::EventArgs
	{
		IncreasedScoreEventArgs(int _newScore) :newScore{ _newScore } {};
		int newScore;
	};
}