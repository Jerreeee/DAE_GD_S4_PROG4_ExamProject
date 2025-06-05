#include "Asset/ResourceManager.h"
#include "Asset/Texture2D.h"
#include "Math/Rect.h"
#include "Asset/Sprite.h"
#include "Asset/SpriteEditor.h"

namespace JRE
{
    std::vector<AssetRef<Sprite>> SpriteEditor::SplitTexture2D(AssetRef<Texture2D> textureRef, int nrSprites, int cols, int rows)
    {
        if (!textureRef)
            throw std::runtime_error("SpriteEditor::SplitTexture2D() | AssetRef<Texture2D> textureRef was nullptr");

        glm::ivec2 size = textureRef->GetSize();
        int spriteWidth = static_cast<int>(size.x / cols);
        int spriteHeight = static_cast<int>(size.y / rows);

        std::vector<AssetRef<Sprite>> sprites{};
        for (int spriteIdx{}; spriteIdx < nrSprites; ++spriteIdx)
        {
            int r = static_cast<int>(spriteIdx / cols);
            int c = spriteIdx % cols;
            float x = static_cast<float>(c * spriteWidth);
            float y = static_cast<float>(r * spriteHeight);
            Region region{ x, y, static_cast<float>(spriteWidth), static_cast<float>(spriteHeight) };
            SoftAssetRef<Texture2D> softTextureRef{ textureRef };
            sprites.emplace_back(CreateAssetRef<Sprite>(softTextureRef, region));
        }
        return sprites;
    }
}
