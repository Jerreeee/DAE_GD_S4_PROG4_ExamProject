#pragma once
#include "InputUtils.h"

namespace JREngine
{
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};
}