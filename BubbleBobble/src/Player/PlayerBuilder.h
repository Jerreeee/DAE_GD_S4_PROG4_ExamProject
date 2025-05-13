#pragma once
#include <memory>
#include <filesystem>

namespace JRE
{
	class GameObject;
}

namespace BubbleBobble::Player
{
	class ScriptComponent;
	class Builder final
	{
	public:
		Builder& SetAnimationPath(const std::filesystem::path& path);
		void Build(std::unique_ptr<JRE::GameObject>& player);
	private:
		void SetAnimations(ScriptComponent* pScriptCmp);

		std::filesystem::path m_AnimPath{};
	};
}