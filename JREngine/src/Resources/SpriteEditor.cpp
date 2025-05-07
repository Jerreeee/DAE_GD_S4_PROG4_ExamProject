#include "Resources/ResourceManager.h"
#include "Rendering/Texture2D.h"
#include "Math/Rect.h"
#include "Rendering/Sprite.h"
#include "Resources/SpriteEditor.h"

namespace JRE
{
    //Sprite sprite(textureHandle);
    //AssetHandle handle = sprite.GetHandle();
    //auto fakePath = std::filesystem::path("generated/Sprite" + '@' + std::to_string(uint64_t(textureHandle)));
    //ResourceManager::GetActive().AddAsset(std::move(sprite), fakePath);
    //return handle;

    std::vector<AssetHandle> SpriteEditor::SplitTexture2D(AssetHandle textureHandle, int nrSprites, int cols, int rows)
    {
        Ref<Texture2D> tex = ResourceManager::GetAsset<Texture2D>(textureHandle);
        glm::ivec2 size = tex->GetSize();
        int spriteWidth = static_cast<int>(size.x / cols);
        int spriteHeight = static_cast<int>(size.y / rows);

        std::vector<AssetHandle> assetHandles{};
        for (int spriteIdx{}; spriteIdx < nrSprites; ++spriteIdx)
        {
            int r = static_cast<int>(spriteIdx / cols);
            int c = spriteIdx % cols;
            int x = c * spriteWidth;
            int y = r * spriteHeight;
            Region region{ x, y, spriteWidth, spriteHeight };
            Sprite sprite(textureHandle, region);
            assetHandles.emplace_back(sprite.GetHandle());
            auto fakePath = std::filesystem::path("generated/Sprite" + '@' + std::to_string(uint64_t(textureHandle)) + '@' + std::to_string(spriteIdx));
        }
        return assetHandles;
    }
}