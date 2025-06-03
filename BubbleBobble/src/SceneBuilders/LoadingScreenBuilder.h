#pragma once

namespace JRE
{
	class Scene;
}

namespace BubbleBobble
{
	class LoadingMenuBuilder final
	{
	public:
		LoadingMenuBuilder(JRE::Scene& scene);
		void Build();
	private:
		JRE::Scene& m_Scene;
	};
}
