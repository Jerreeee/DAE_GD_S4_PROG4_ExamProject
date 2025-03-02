#include "TestTrashTheCashComponent.h"
#include "Renderer.h"

dae::TestTrashTheCashComponent::TestTrashTheCashComponent(GameObject& gameObject) :
	UIComponentBase(gameObject)
{
}

void dae::TestTrashTheCashComponent::UpdateAndRenderUI()
{
	RenderExercise1();
	RenderExercise2();
}

void dae::TestTrashTheCashComponent::RenderExercise1()
{
	float titleBarheight = ImGui::GetFrameHeightWithSpacing();
	ImGui::SetNextWindowContentSize(ImVec2{ 200, 150 + titleBarheight });
	ImGui::Begin("Exercise 1");

	ImGui::PlotConfig conf{};
	conf.values.color = m_IntPlotColor;
	conf.values.ys = m_IntTimings.data();
	conf.values.count = int(m_IntTimings.size());
	conf.scale.min = 0;
	conf.scale.max = m_MaxIntTiming;
	conf.tooltip.show = true;
	conf.tooltip.format = "x=%.2f, y=%.2f";
	conf.frame_size = ImVec2(200, 100);
	conf.line_thickness = 2.f;

	ImGui::SliderInt("Sampels", &m_IntSamples, 1, 10);
	ImGui::SliderInt("Data Count", &m_IntDataCount, 100'000, 100'000'000);
	if (ImGui::Button("Trash The Cache"))
	{
		ImGui::Text("Wait for it...");
		GenerateEx1Data();
	}
	else
	{
		ImGui::Text("Int");
		ImGui::Plot("Int plot", conf);
	}

	ImGui::End();
}

void dae::TestTrashTheCashComponent::RenderExercise2()
{
	float titleBarheight = ImGui::GetFrameHeightWithSpacing();
	ImGui::SetNextWindowContentSize(ImVec2{ 200, 300 + titleBarheight });
	ImGui::Begin("Exercise 2");

	ImGui::PlotConfig conf{};
	conf.values.count = int(m_GameObjectTimings.size());
	conf.scale.min = 0;
	conf.tooltip.show = true;
	conf.tooltip.format = "x=%.2f, y=%.2f";
	conf.frame_size = ImVec2(200, 100);
	conf.line_thickness = 2.f;

	ImGui::SliderInt("Sampels", &m_GameObjectSamples, 1, 10);
	ImGui::SliderInt("Data Count", &m_GameObjectDataCount, 100'000, 100'000'000);
	if (ImGui::Button("Trash The Cache"))
	{
		ImGui::Text("Wait for it...");
		GenerateEx2Data();
	}
	else
	{
		ImGui::Text("GameObject3D");
		conf.values.color = m_GameObjectPlotColor;
		conf.values.ys = m_GameObjectTimings.data();
		conf.scale.max = m_MaxGameObjectTiming;
		ImGui::Plot("GameObject3D plot", conf);

		ImGui::Text("GameObject3DAlt");
		conf.values.color = m_GameObjectAltPlotColor;
		conf.values.ys = m_GameObjectAltTimings.data();
		conf.scale.max = m_MaxGameObjectAltTiming;
		ImGui::Plot("GameObject3DAlt plot", conf);

		ImGui::Text("Combined GameObject3D");
		conf.values.color = 0;
		conf.values.colors = m_CombinedGameObjectPlotColors.data();
		conf.values.ys = nullptr;
		conf.values.ys_list = m_CombinedGameObjectTimings.data();
		conf.values.ys_count = int(m_CombinedGameObjectTimings.size());
		conf.scale.max = m_MaxGameObjectTiming;
		ImGui::Plot("Combined GameObject3D plot", conf);
	}

	ImGui::End();
}

void dae::TestTrashTheCashComponent::GenerateEx1Data()
{
	GenerateData<int>(m_IntDataCount, m_IntTimings, m_MaxIntTiming, m_IntSamples, m_NrSamplesRemovedPerEnd,
		[](int& value) {
			value *= 2;
		}
	);
}

void dae::TestTrashTheCashComponent::GenerateEx2Data()
{
	GenerateData<GameObject3D>(m_GameObjectDataCount, m_GameObjectTimings, m_MaxGameObjectTiming, m_GameObjectSamples, m_NrSamplesRemovedPerEnd,
		[](GameObject3D& value) {
			value.id *= 2;
		}
	);

	GenerateData<GameObject3DAlt>(m_GameObjectDataCount, m_GameObjectAltTimings, m_MaxGameObjectAltTiming, m_GameObjectSamples, m_NrSamplesRemovedPerEnd,
		[](GameObject3DAlt& value) {
			value.id *= 2;
		}
	);

	m_CombinedGameObjectTimings.clear();
	m_CombinedGameObjectTimings.emplace_back(m_GameObjectTimings.data());
	m_CombinedGameObjectTimings.emplace_back(m_GameObjectAltTimings.data());
	m_MaxCombinedGameObjectTiming = std::max(m_MaxGameObjectTiming, m_MaxGameObjectAltTiming);
}
