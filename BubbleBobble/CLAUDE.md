# CLAUDE.md — BubbleBobble

Game executable built on JREngine. A Bubble Bobble clone.

## Game Architecture

Entry point: `src/Main.cpp` — creates engine, GameManager, scene builders, then launches.

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

## Data Folder
`Data/` contains all runtime assets (textures, sounds, fonts, level data). Copied next to .exe by post-build step.
