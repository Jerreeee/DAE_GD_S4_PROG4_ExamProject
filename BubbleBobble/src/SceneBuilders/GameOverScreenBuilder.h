#pragma once

namespace BubbleBobble
{
	class GameOverScreenBuilder final
	{
	public:
		GameOverScreenBuilder(JRE::Scene& scene);
		void Build();
	private:
		JRE::Scene& m_Scene;
	};
}
