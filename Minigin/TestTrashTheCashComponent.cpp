#include "TestTrashTheCashComponent.h"
#include "Renderer.h"
#include "imgui_plot.h"

dae::TestTrashTheCashComponent::TestTrashTheCashComponent(GameObject& gameObject) :
	RendererComponentBase(gameObject)
{
	GenerateData();
}

void dae::TestTrashTheCashComponent::Render() const
{
	ImGui::Begin("Plot window");

	ImGui::PlotConfig conf{};
	conf.values.xs = x_data; // this line is optional
	conf.values.ys = y_data1;
	conf.values.count = data_count;
	conf.scale.min = -1;
	conf.scale.max = 1;
	conf.tooltip.show = true;
	conf.tooltip.format = "x=%.2f, y=%.2f";
	conf.grid_x.show = true;
	conf.grid_y.show = true;
	conf.frame_size = ImVec2(400, 400);
	conf.line_thickness = 2.f;

	ImGui::Plot("plot", conf);

	ImGui::End();
}

void dae::TestTrashTheCashComponent::GenerateData()
{
	constexpr float sampling_freq = 44100;
	constexpr float freq = 500;
	for (size_t i = 0; i < data_count; ++i) {
		const float t = i / sampling_freq;
		x_data[i] = t;
		const float arg = float(2.f * M_PI * freq * t);
		y_data1[i] = sin(arg);
		y_data2[i] = y_data1[i] * -0.6f + sin(2 * arg) * 0.4f;
		y_data3[i] = y_data2[i] * -0.6f + sin(3 * arg) * 0.4f;
	}
}
