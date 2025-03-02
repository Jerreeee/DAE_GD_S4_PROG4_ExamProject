#pragma once
#include "UIComponentBase.h"
#include <vector>
#include <functional>
#include <chrono>
#include <algorithm>
#include <numeric>
#include "imgui_plot.h"

template<typename Value_t>
concept DefaultInitializable = std::default_initializable<Value_t>;

template<typename Func, typename Value_t>
concept CallableWithType = requires(Func func, Value_t& value_t) {
	{ func(value_t) } -> std::same_as<void>;
};

namespace dae
{
	class TestTrashTheCashComponent final : public UIComponentBase
	{
	public:
		TestTrashTheCashComponent(GameObject& gameObject);

		virtual void UpdateAndRenderUI() override;
	private:
		//---------------------------
		//Types
		//---------------------------
		struct transform
		{
			float matrix[16] = {
				1,0,0,0,
				0,1,0,0,
				0,0,1,0,
				0,0,0,1
			};
		};

		class GameObject3D
		{
		public:
			transform local{};
			int id{};
		};

		class GameObject3DAlt
		{
		public:
			transform* local{};
			int id{};
		};

		//---------------------------
		//Functions
		//---------------------------
		void RenderExercise1();
		void RenderExercise2();

		void GenerateEx1Data();
		void GenerateEx2Data();

		template <DefaultInitializable Value_t, CallableWithType<Value_t> Func>
		void GenerateData(
			int dataCount,
			std::vector<float>& timings,
			float& maxTime,
			int nrSamples,
			int nrSamplesRemovedPerEnd,
			Func func) //Important to use templated function parameter here to ensure inlining
		{
			Value_t* dataArr = new Value_t[dataCount]{};

			timings.clear();

			//ensure begin < end in std::accumulate(begin, end)
			nrSamplesRemovedPerEnd = std::min(nrSamplesRemovedPerEnd, int((nrSamples - 1) / 2.f));
			int nrSamplesForAvgTime{ nrSamples - 2 * nrSamplesRemovedPerEnd };
			std::vector<float> samples{};
			samples.resize(nrSamples);

			for (int stepSize = 1; stepSize <= 1024; stepSize *= 2)
			{
				for (int sampleIdx{}; sampleIdx < samples.size(); ++sampleIdx)
				{
					auto start = std::chrono::high_resolution_clock::now();
					for (int dataIdx = 0; dataIdx < dataCount; dataIdx += stepSize)
					{
						func(dataArr[dataIdx]);
					}
					auto end = std::chrono::high_resolution_clock::now();
					auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
					samples[sampleIdx] = float(time);
				}
				std::sort(samples.begin(), samples.end());
				float accTime = std::accumulate(samples.begin() + nrSamplesRemovedPerEnd, samples.end() - nrSamplesRemovedPerEnd, 0.f);
				float avgTime = accTime / nrSamplesForAvgTime;
				timings.emplace_back(avgTime);
			}

			auto m_MaxTimingIt = std::max_element(timings.begin(), timings.end());
			if (m_MaxTimingIt != timings.end())
				maxTime = *m_MaxTimingIt;

			delete[] dataArr;
		}

		//---------------------------
		//Variables
		//---------------------------
		int m_NrSamplesRemovedPerEnd{ 2 };

		//exercise 1
		int m_IntSamples{ 10 };
		int m_IntDataCount{ 50'000'000 };
		const ImColor m_IntPlotColor{ 255, 255, 0 };
		std::vector<float> m_IntTimings{};
		float m_MaxIntTiming{};

		//exercise 2
		int m_GameObjectSamples{ 10 };
		int m_GameObjectDataCount{ 50'000'000 };
		ImU32 m_GameObjectPlotColor{ ImColor{0, 255, 0} };
		ImU32 m_GameObjectAltPlotColor{ ImColor{255, 0, 0} };
		std::vector<ImU32> m_CombinedGameObjectPlotColors{ m_GameObjectPlotColor , m_GameObjectAltPlotColor };

		std::vector<float> m_GameObjectTimings{};
		float m_MaxGameObjectTiming{};

		std::vector<float> m_GameObjectAltTimings{};
		float m_MaxGameObjectAltTiming{};

		float m_MaxCombinedGameObjectTiming{};
		std::vector<const float*> m_CombinedGameObjectTimings{};
	};
}