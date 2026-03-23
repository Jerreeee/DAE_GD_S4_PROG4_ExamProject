#include "JREngine/Asset/AssetDatabase.h"
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
        auto& ai = AssetDatabase::GetInstance();

        // ── FONTS ─────────────────────────────────────────────────────────────
        {
            auto f20 = FontImporter("Fonts/Pixel_NES.otf"); f20.SetSize(20);
            (void)ai.RegisterAsset(std::move(f20));

            auto f30 = FontImporter("Fonts/Pixel_NES.otf"); f30.SetSize(30);
            (void)ai.RegisterAsset(std::move(f30));
        }

        // ── MUSIC ─────────────────────────────────────────────────────────────
        (void)ai.RegisterAsset(SoundMusicImporter("MainTheme.mp3"));

        // ── SOUND EFFECTS ─────────────────────────────────────────────────────
        (void)ai.RegisterAsset(SoundClipImporter("HUD/Opening.wav"));
        (void)ai.RegisterAsset(SoundClipImporter("HUD/Select.wav"));
        (void)ai.RegisterAsset(SoundClipImporter("Player/Jump.wav"));
        (void)ai.RegisterAsset(SoundClipImporter("Player/Shoot.wav"));

        // ── HUD TEXTURES ──────────────────────────────────────────────────────
        (void)ai.RegisterAsset(TextureImporter("HUD/Health_P1.png"));

        // ── LOGO ──────────────────────────────────────────────────────────────
        (void)ai.RegisterAsset(TextureImporter("HUD/Logo_Anim.png"));
        (void)ai.RegisterAsset(AnimDataImporter("Anims/Logo.txt"));

        // ── PLAYER BUBBY (P1) ─────────────────────────────────────────────────
        AssetHandle bubbyIdle   = ai.RegisterAsset(TextureImporter("Player/Bubby/Idle_Anim.png"));
        AssetHandle bubbyRun    = ai.RegisterAsset(TextureImporter("Player/Bubby/Run_Anim.png"));
        AssetHandle bubbyShoot  = ai.RegisterAsset(TextureImporter("Player/Bubby/Shoot_Anim.png"));
        AssetHandle bubbyDeath  = ai.RegisterAsset(TextureImporter("Player/Bubby/Death_Anim.png"));
        AssetHandle bubbyIntro  = ai.RegisterAsset(TextureImporter("Player/Bubby/Intro_Anim.png"));
        AssetHandle bubbyBurn   = ai.RegisterAsset(TextureImporter("Player/Bubby/Burn_Anim.png"));
        AssetHandle bubbyBubble = ai.RegisterAsset(TextureImporter("Player/Bubby/Bubble_Anim.png"));

        (void)ai.RegisterAsset(std::move(AnimDataImporter("Anims/P1.txt")
            .DependsOn(bubbyIdle).DependsOn(bubbyRun)
            .DependsOn(bubbyShoot).DependsOn(bubbyDeath)));
        (void)ai.RegisterAsset(std::move(AnimDataImporter("Anims/Intro_P1.txt").DependsOn(bubbyIntro)));
        (void)ai.RegisterAsset(std::move(AnimDataImporter("Anims/Fail_P1.txt").DependsOn(bubbyBurn)));

        // ── PLAYER BOBBY (P2 — intro/fail only) ──────────────────────────────
        AssetHandle bobbyIntro  = ai.RegisterAsset(TextureImporter("Player/Bobby/Intro_Anim.png"));
        AssetHandle bobbyBurn   = ai.RegisterAsset(TextureImporter("Player/Bobby/Burn_Anim.png"));
        AssetHandle bobbyBubble = ai.RegisterAsset(TextureImporter("Player/Bobby/Bubble_Anim.png"));

        (void)ai.RegisterAsset(std::move(AnimDataImporter("Anims/Intro_P2.txt").DependsOn(bobbyIntro)));
        (void)ai.RegisterAsset(std::move(AnimDataImporter("Anims/Fail_P2.txt").DependsOn(bobbyBurn)));

        // ── BUBBLES ───────────────────────────────────────────────────────────
        AssetHandle bubble2    = ai.RegisterAsset(TextureImporter("Player/Bubble_Anim2.png"));
        AssetHandle bubble3    = ai.RegisterAsset(TextureImporter("Player/Bubble_Anim3.png"));
        AssetHandle bubblePoof = ai.RegisterAsset(TextureImporter("Player/Bubble_Poof.png"));

        (void)ai.RegisterAsset(std::move(AnimDataImporter("Anims/Bubble.txt")
            .DependsOn(bubbyBubble).DependsOn(bobbyBubble)
            .DependsOn(bubble2).DependsOn(bubble3).DependsOn(bubblePoof)));

        // ── ZENCHAN ENEMY ─────────────────────────────────────────────────────
        AssetHandle zenRun    = ai.RegisterAsset(TextureImporter("Enemies/ZenChan/Run_Anim.png"));
        AssetHandle zenAngry  = ai.RegisterAsset(TextureImporter("Enemies/ZenChan/RunAngry_Anim.png"));
        AssetHandle zenBub    = ai.RegisterAsset(TextureImporter("Enemies/ZenChan/Bubble_Anim.png"));
        AssetHandle zenBubAlt = ai.RegisterAsset(TextureImporter("Enemies/ZenChan/Bubble_Anim_Alt.png"));
        AssetHandle zenBub2   = ai.RegisterAsset(TextureImporter("Enemies/ZenChan/Bubble_Anim2.png"));
        AssetHandle zenBub3   = ai.RegisterAsset(TextureImporter("Enemies/ZenChan/Bubble_Anim3.png"));
        AssetHandle zenDeath  = ai.RegisterAsset(TextureImporter("Enemies/ZenChan/Death_Anim.png"));

        (void)ai.RegisterAsset(std::move(AnimDataImporter("Anims/ZenChan.txt")
            .DependsOn(zenRun).DependsOn(zenAngry).DependsOn(zenBub)
            .DependsOn(zenBubAlt).DependsOn(zenBub2).DependsOn(zenBub3)
            .DependsOn(zenDeath)));

        // ── LEVELS ────────────────────────────────────────────────────────────
        for (int i = 1; i <= 3; ++i)
        {
            std::string base = "Levels/" + std::to_string(i) + "/";
            AssetHandle bigTex   = ai.RegisterAsset(TextureImporter(base + "Big.png"));
            AssetHandle smallTex = ai.RegisterAsset(TextureImporter(base + "Small.png"));

            (void)ai.RegisterAsset(std::move(TileMapImporter(base + "TileMapData.txt")
                .DependsOn(bigTex).DependsOn(smallTex)));
            (void)ai.RegisterAsset(LevelDataImporter(base + "LevelData.txt"));
        }

        // ── SCENE DESCRIPTORS ────────────────────────────────────────────────────────
        // Must be LAST — GetMetadata() resolves paths via AssetRegistry::GetHandleAtPath()
        // which only works after all referenced assets above are already registered.
        (void)ai.RegisterAsset(SceneDescriptorImporter("Scenes/MainMenu.scene"));
        (void)ai.RegisterAsset(SceneDescriptorImporter("Scenes/LoadingScreen.scene"));
        (void)ai.RegisterAsset(SceneDescriptorImporter("Scenes/GameOver.scene"));
        (void)ai.RegisterAsset(SceneDescriptorImporter("Scenes/HUD.scene"));
        for (int i = 1; i <= 3; ++i)
            (void)ai.RegisterAsset(SceneDescriptorImporter("Scenes/Level" + std::to_string(i) + ".scene"));
    }
}
