#pragma once
#include <string>
#include <functional>
#include <filesystem>
#include <chrono>

namespace JRE
{
	class JREngine final
	{
		bool m_Quit{};
		float m_FixedTimeStep{ 1.f / 60.f };
		std::chrono::steady_clock::time_point m_LastTime;
		float m_Lag{};
	public:
		explicit JREngine(const std::filesystem::path& dataPath);
		~JREngine();
		void Run(const std::function<void()>& load);
		void RunOneFrame();

		JREngine(const JREngine& other) = delete;
		JREngine(JREngine&& other) = delete;
		JREngine& operator=(const JREngine& other) = delete;
		JREngine& operator=(JREngine&& other) = delete;
	};
}