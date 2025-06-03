#include "SDL.h"
#undef main
#include "JREngine/Scene/GameObject.h"
#include "JREngine/Rendering/TextRendererComponent.h"
#include "JREngine/Asset/Sprite.h"

#include "SceneBuilders/BuilderHelpers.h"

namespace BubbleBobble
{
	void BuilderHelpers::AddCenteredTxt(JRE::Scene& scene, const std::string& text, JRE::SoftAssetRef<JRE::Font> font, float centerX, float y, const SDL_Color& color)
	{
		auto go = std::make_unique<JRE::GameObject>(text);
		auto cmp = go->AddComponent<JRE::TextRendererComponent>(text, font, color);
		const JRE::Region& region = cmp->GetSprite().GetSrcRegion();
		float x = centerX - region.width / 2;
		go->SetWorldPosition(x, y);
		scene.Add(std::move(go));
	}
}
