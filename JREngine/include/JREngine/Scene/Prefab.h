#pragma once
#include <functional>
#include <memory>
#include "JREngine/Scene/GameObject.h"
#include "JREngine/Scene/SceneManager.h"

namespace JRE
{
	class Prefab
	{
	public:
		using CreatorFunc = std::function<std::unique_ptr<GameObject>(const std::string&)>;

		Prefab(const CreatorFunc& creator)
			: m_Creator{ creator }
		{}

		std::unique_ptr<GameObject> Instantiate(const std::string& gameObjectName) const
		{
			return std::move(m_Creator(gameObjectName));
		}
	private:
		CreatorFunc m_Creator;
	};
}
