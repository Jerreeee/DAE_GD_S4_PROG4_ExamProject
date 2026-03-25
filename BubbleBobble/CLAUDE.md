# CLAUDE.md — BubbleBobble

Game executable built on JRRuntime. A Bubble Bobble clone.

**Links:** JRRuntime (privately) → gets JRCore transitively.

## Entry point & startup flow

`src/Main.cpp`:
1. Locate `Data/` and resolve `asset_manifest.txt` path
2. Exit with a clear error if the manifest is missing (run `JREditor.exe` first)
3. Construct `JREngine`, register `RuntimeResourceManager` with the manifest
4. Call `engine.Run(buildScenes)` — enters the game loop

The **editor phase** (asset registration, manifest writing) is **not** done here.
That is `JREditor.exe`'s job. `JREditor.exe` is run automatically as a POST_BUILD
step after every BubbleBobble build (see `BubbleBobble/CMakeLists.txt`).

## Asset manifest

`BubbleBobble/src/Assets/AssetManifest.cpp` contains `AssetManifest::RegisterAll()` —
the complete list of game assets. This file is **compiled into JREditor**, not into
BubbleBobble. BubbleBobble does not link JREditor and has no knowledge of asset registration.

If you add a new asset, update `AssetManifest::RegisterAll()` and rebuild — the POST_BUILD
step will regenerate the manifest automatically.

## Game Architecture

### Game States (GameManagerComponent)
MainMenu → LoadingScreen → InGame → GameOverScreen. Each state is an `IGameState` that builds its scene via a corresponding builder in `SceneBuilders/`.

### Scene Builders
- `MainMenuBuilder`: Menu UI and input bindings
- `LoadingMenuBuilder`: Asset preloading screen
- `LevelBuilder`: Constructs level from LevelData — player, tilemap, enemies, UI
- `GameOverScreenBuilder`: Score display and restart
- `UIBuilder`: Shared HUD elements

### Player System
- `PlayerScriptComponent`: Core logic, state management, IObserver for events
- `PlayerControllerComponent`: Bridges InputManager actions to player methods
- `PlayerStates.h`: Mortal/Immortal/Dead states (deferred transitions)
- `PlayerBuilder`: Factory for creating player GameObjects

### Enemy System
- `ZenchanScriptComponent`: Enemy behavior
- `ZenchanAIController`: Patrol/chase/flee AI
- `ZenchanBuilder`: Factory for enemy GameObjects

### Collision Layers (EngineSetup.h)
Friendly (1<<0), Enemy (1<<1), StaticLevel (1<<2), Platform (1<<3), Bubble (1<<4). Collision masks define which layers interact.

### Persistence Layers (EngineSetup.h)
Global (1<<2), Levels (1<<3). Combined into masks: PlayerAndUI, LevelScene, MenuScene.

### Custom Assets
- `TileMap` / `TileMapImporter`: Level geometry
- `LevelData` / `LevelDataImporter`: Enemy placement, platform data
- `AnimsData` / `AnimsDataImporter`: Animation configuration
- `SceneDescriptor` / `SceneDescriptorImporter`: Serialised scene layouts

## Data Folder
`Data/` contains all runtime assets (textures, sounds, fonts, level data). Copied next to the
exe by post-build step, followed immediately by the manifest-generation step.
