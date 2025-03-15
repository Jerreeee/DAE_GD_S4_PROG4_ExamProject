#pragma once
#include <string>
#include <functional>
#include <filesystem>
#include <chrono>

namespace Engine
{
	class Minigin final
	{
		bool m_Quit{};
		float m_FixedTimeStep{ 1.f / 60.f };
		std::chrono::steady_clock::time_point m_LastTime;
	public:
		explicit Minigin(const std::filesystem::path& dataPath);
		~Minigin();
		void Run(const std::function<void()>& load);
		void RunOneFrame();

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;
	};
}