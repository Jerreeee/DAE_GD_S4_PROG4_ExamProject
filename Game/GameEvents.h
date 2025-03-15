#pragma once
#include "../Minigin/Event.h"

namespace Game
{
	struct PlayerDamagedEventArgs : public Engine::EventArgs
	{
		explicit PlayerDamagedEventArgs(int _damage, int _newHealth) :
			damage{ _damage }, newhealth{ _newHealth }
		{};
		int damage;
		int newhealth;
	};
}