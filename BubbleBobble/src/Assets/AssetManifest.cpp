#include "JREngine/Asset/AssetImporter.h"
#include "JREngine/Asset/TextureImporter.h"
#include "JREngine/Asset/FontImporter.h"
#include "JREngine/Asset/SoundClipImporter.h"
#include "JREngine/Asset/SoundMusicImporter.h"
#include "Assets/AnimsDataImporter.h"
#include "Assets/LevelDataImporter.h"
#include "TileMap/TileMapImporter.h"
#include "Assets/SceneDescriptorImporter.h"
#include "Assets/AssetManifest.h"

using namespace JRE;

namespace BubbleBobble
{
    void AssetManifest::RegisterAll()
    {
        auto& ai = AssetImporter::GetInstance();

        // ── FONTS ─────────────────────────────────────────────────────────────
        {
            auto f20 = FontImporter("Fonts/Pixel_NES.otf"); f20.SetSize(20);
            (void)ai.ImportAsset(std::move(f20));

            auto f30 = FontImporter("Fonts/Pixel_NES.otf"); f30.SetSize(30);
            (void)ai.ImportAsset(std::move(f30));
        }

        // ── MUSIC ─────────────────────────────────────────────────────────────
        (void)ai.ImportAsset(SoundMusicImporter("MainTheme.mp3"));

        // ── SOUND EFFECTS ─────────────────────────────────────────────────────
        (void)ai.ImportAsset(SoundClipImporter("HUD/Opening.wav"));
        (void)ai.ImportAsset(SoundClipImporter("HUD/Select.wav"));
        (void)ai.ImportAsset(SoundClipImporter("Player/Jump.wav"));
        (void)ai.ImportAsset(SoundClipImporter("Player/Shoot.wav"));

        // ── HUD TEXTURES ──────────────────────────────────────────────────────
        (void)ai.ImportAsset(TextureImporter("HUD/Health_P1.png"));

        // ── LOGO ──────────────────────────────────────────────────────────────
        (void)ai.ImportAsset(TextureImporter("HUD/Logo_Anim.png"));
        (void)ai.ImportAsset(AnimDataImporter("Anims/Logo.txt"));

        // ── PLAYER BUBBY (P1) ─────────────────────────────────────────────────
        AssetHandle bubbyIdle   = ai.ImportAsset(TextureImporter("Player/Bubby/Idle_Anim.png"));
        AssetHandle bubbyRun    = ai.ImportAsset(TextureImporter("Player/Bubby/Run_Anim.png"));
        AssetHandle bubbyShoot  = ai.ImportAsset(TextureImporter("Player/Bubby/Shoot_Anim.png"));
        AssetHandle bubbyDeath  = ai.ImportAsset(TextureImporter("Player/Bubby/Death_Anim.png"));
        AssetHandle bubbyIntro  = ai.ImportAsset(TextureImporter("Player/Bubby/Intro_Anim.png"));
        AssetHandle bubbyBurn   = ai.ImportAsset(TextureImporter("Player/Bubby/Burn_Anim.png"));
        AssetHandle bubbyBubble = ai.ImportAsset(TextureImporter("Player/Bubby/Bubble_Anim.png"));

        (void)ai.ImportAsset(std::move(AnimDataImporter("Anims/P1.txt")
            .DependsOn(bubbyIdle).DependsOn(bubbyRun)
            .DependsOn(bubbyShoot).DependsOn(bubbyDeath)));
        (void)ai.ImportAsset(std::move(AnimDataImporter("Anims/Intro_P1.txt").DependsOn(bubbyIntro)));
        (void)ai.ImportAsset(std::move(AnimDataImporter("Anims/Fail_P1.txt").DependsOn(bubbyBurn)));

        // ── PLAYER BOBBY (P2 — intro/fail only) ──────────────────────────────
        AssetHandle bobbyIntro  = ai.ImportAsset(TextureImporter("Player/Bobby/Intro_Anim.png"));
        AssetHandle bobbyBurn   = ai.ImportAsset(TextureImporter("Player/Bobby/Burn_Anim.png"));
        AssetHandle bobbyBubble = ai.ImportAsset(TextureImporter("Player/Bobby/Bubble_Anim.png"));

        (void)ai.ImportAsset(std::move(AnimDataImporter("Anims/Intro_P2.txt").DependsOn(bobbyIntro)));
        (void)ai.ImportAsset(std::move(AnimDataImporter("Anims/Fail_P2.txt").DependsOn(bobbyBurn)));

        // ── BUBBLES ───────────────────────────────────────────────────────────
        AssetHandle bubble2    = ai.ImportAsset(TextureImporter("Player/Bubble_Anim2.png"));
        AssetHandle bubble3    = ai.ImportAsset(TextureImporter("Player/Bubble_Anim3.png"));
        AssetHandle bubblePoof = ai.ImportAsset(TextureImporter("Player/Bubble_Poof.png"));

        (void)ai.ImportAsset(std::move(AnimDataImporter("Anims/Bubble.txt")
            .DependsOn(bubbyBubble).DependsOn(bobbyBubble)
            .DependsOn(bubble2).DependsOn(bubble3).DependsOn(bubblePoof)));

        // ── ZENCHAN ENEMY ─────────────────────────────────────────────────────
        AssetHandle zenRun    = ai.ImportAsset(TextureImporter("Enemies/ZenChan/Run_Anim.png"));
        AssetHandle zenAngry  = ai.ImportAsset(TextureImporter("Enemies/ZenChan/RunAngry_Anim.png"));
        AssetHandle zenBub    = ai.ImportAsset(TextureImporter("Enemies/ZenChan/Bubble_Anim.png"));
        AssetHandle zenBubAlt = ai.ImportAsset(TextureImporter("Enemies/ZenChan/Bubble_Anim_Alt.png"));
        AssetHandle zenBub2   = ai.ImportAsset(TextureImporter("Enemies/ZenChan/Bubble_Anim2.png"));
        AssetHandle zenBub3   = ai.ImportAsset(TextureImporter("Enemies/ZenChan/Bubble_Anim3.png"));
        AssetHandle zenDeath  = ai.ImportAsset(TextureImporter("Enemies/ZenChan/Death_Anim.png"));

        (void)ai.ImportAsset(std::move(AnimDataImporter("Anims/ZenChan.txt")
            .DependsOn(zenRun).DependsOn(zenAngry).DependsOn(zenBub)
            .DependsOn(zenBubAlt).DependsOn(zenBub2).DependsOn(zenBub3)
            .DependsOn(zenDeath)));

        // ── LEVELS ────────────────────────────────────────────────────────────
        for (int i = 1; i <= 3; ++i)
        {
            std::string base = "Levels/" + std::to_string(i) + "/";
            AssetHandle bigTex   = ai.ImportAsset(TextureImporter(base + "Big.png"));
            AssetHandle smallTex = ai.ImportAsset(TextureImporter(base + "Small.png"));

            (void)ai.ImportAsset(std::move(TileMapImporter(base + "TileMapData.txt")
                .DependsOn(bigTex).DependsOn(smallTex)));
            (void)ai.ImportAsset(LevelDataImporter(base + "LevelData.txt"));
        }

        // ── SCENE DESCRIPTORS ────────────────────────────────────────────────────────
        // Must be LAST — GetMetadata() resolves paths via AssetRegistry::GetHandleAtPath()
        // which only works after all referenced assets above are already registered.
        (void)ai.ImportAsset(SceneDescriptorImporter("Scenes/MainMenu.scene"));
        (void)ai.ImportAsset(SceneDescriptorImporter("Scenes/LoadingScreen.scene"));
        (void)ai.ImportAsset(SceneDescriptorImporter("Scenes/GameOver.scene"));
        (void)ai.ImportAsset(SceneDescriptorImporter("Scenes/HUD.scene"));
        for (int i = 1; i <= 3; ++i)
            (void)ai.ImportAsset(SceneDescriptorImporter("Scenes/Level" + std::to_string(i) + ".scene"));
    }
}
