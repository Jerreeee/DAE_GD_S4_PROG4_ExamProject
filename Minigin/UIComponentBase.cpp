#include "UIComponentBase.h"
#include "Renderer.h"

dae::UIComponentBase::UIComponentBase(GameObject& gameObject) :
	ComponentBase(gameObject)
{
	Renderer::GetInstance().RegisterUIComponent(this);
}

dae::UIComponentBase::~UIComponentBase()
{
	Renderer::GetInstance().UnRegisterUIComponent(this);
}
