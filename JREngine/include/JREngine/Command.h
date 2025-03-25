#pragma once
#include "InputUtils.h"

namespace Engine
{
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};
}