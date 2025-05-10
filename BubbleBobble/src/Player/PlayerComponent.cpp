#include "JREngine/Scene/GameObject.h"
#include "Player/PlayerComponent.h"

namespace BubbleBobble
{
	PlayerComponent::PlayerComponent(GameObject& gameObject) :
		ComponentBase(GameObject& gameObject)
	{
	}
}
