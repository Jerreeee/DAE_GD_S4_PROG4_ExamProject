#pragma once
#include "InputUtils.h"

namespace JRE
{
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};
}