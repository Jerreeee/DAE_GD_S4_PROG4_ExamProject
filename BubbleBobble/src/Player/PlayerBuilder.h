#pragma once
#include <memory>
#include <filesystem>

namespace JRE
{
	class GameObject;
	struct Input::ActionMap;
	class SpriteAnimatorComponent;
}

namespace BubbleBobble
{
	class PlayerBuilder final
	{
	public:
		PlayerBuilder& SetAnimationPath(const std::filesystem::path& path);
		PlayerBuilder& SetActionMapIdx(size_t actionMapIdx);
		void Build(std::unique_ptr<JRE::GameObject>& player);
	private:
		std::filesystem::path m_AnimPath{};
		int m_ActionMapIdx{ -1 };
	};
}