#pragma once
#include <filesystem>
#include "JREngine/Scene/Scene.h"

namespace BubbleBobble
{
	class UIBuilder final
	{
	public:
		//struct HealthUI
		//{
		//	const JRE::GameObject& gameObject;
		//	std::filesystem::path texturePath;
		//};

		UIBuilder(JRE::Scene& scene);
		//UIBuilder& SetHealthTexturePath(const std::filesystem::path& path);
		UIBuilder& SetPlayer1(const JRE::GameObject& gameObject);
		void Build();
	private:
		JRE::Scene& m_Scene;
		const JRE::GameObject* m_pPlayer1{ nullptr };
	};
}
