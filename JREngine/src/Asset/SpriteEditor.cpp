#include "Asset/ResourceManager.h"
#include "Asset/Texture2D.h"
#include "Math/Rect.h"
#include "Asset/Sprite.h"
#include "Asset/SpriteEditor.h"

namespace JRE
{
    //Sprite sprite(textureHandle);
    //AssetHandle handle = sprite.GetHandle();
    //auto fakePath = std::filesystem::path("generated/Sprite" + '@' + std::to_string(uint64_t(textureHandle)));
    //ResourceManager::GetActive().AddAsset(std::move(sprite), fakePath);
    //return handle;

    std::vector<AssetHandle> SpriteEditor::SplitTexture2D(AssetRef<Texture2D> textureRef, int nrSprites, int cols, int rows)
    {
        if (!textureRef)
            throw std::runtime_error("SpriteEditor::SplitTexture2D() | AssetRef<Texture2D> textureRef was nullptr");

        glm::ivec2 size = textureRef->GetSize();
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
            SoftAssetRef<Texture2D> softTextureRef{ textureRef };
            Sprite sprite(softTextureRef, region);
            assetHandles.emplace_back(sprite.GetHandle());
        }
        return assetHandles;
    }
}
