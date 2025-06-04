#pragma once
#include <memory>
#include <filesystem>

namespace JRE
{
	class GameObject;

	namespace Input
	{
		struct ActionMap;
	}
}

namespace BubbleBobble::Player
{
	class ScriptComponent;
	class Builder final
	{
	public:
		Builder& SetAnimationPath(const std::filesystem::path& path);
		Builder& SetActionMap(const JRE::Input::ActionMap& actionMap);
		void Build(std::unique_ptr<JRE::GameObject>& player);
	private:
		void SetAnimations(ScriptComponent* pScriptCmp);

		std::filesystem::path m_AnimPath{};
		const JRE::Input::ActionMap* m_pActionMap{ nullptr };
	};
}