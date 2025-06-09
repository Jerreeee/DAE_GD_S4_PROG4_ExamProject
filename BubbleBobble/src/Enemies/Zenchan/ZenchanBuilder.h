#pragma once
#include <memory>

namespace JRE
{
	class GameObject;
}

namespace BubbleBobble
{
	class ZenchanBuilder final
	{
	public:
		void Build(std::unique_ptr<JRE::GameObject>& zenchan);
	private:
	};
}