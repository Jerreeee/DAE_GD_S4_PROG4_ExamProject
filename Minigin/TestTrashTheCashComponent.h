#pragma once
#include "RendererComponentBase.h"

namespace dae
{
	class TestTrashTheCashComponent final : public RendererComponentBase
	{
	public:
		TestTrashTheCashComponent(GameObject& gameObject);

		virtual void Render() const override;

		void GenerateData();
	private:
		static const size_t data_count = 512;
		float x_data[data_count] = {};
		float y_data1[data_count] = {};
		float y_data2[data_count] = {};
		float y_data3[data_count] = {};
	};
}