#include "JREngine/Scene/Scene.h"
#include "JREngine/Asset/SoftAssetRef.h"
#include "JREngine/Asset/Font.h"

struct SDL_Color;
namespace BubbleBobble
{
	class BuilderHelpers final
	{
	public:
		static void AddCenteredTxt(JRE::Scene& scene, const std::string& text, JRE::SoftAssetRef<JRE::Font> font, float centerX, float y, const SDL_Color& color);
	};
}
