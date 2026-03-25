# CLAUDE.md — JRCore

Pure C++20 static library. **Zero platform dependencies** — no SDL, no XInput, no ImGui.
Only external dependency: **GLM** (header-only math). Can compile and run headlessly.

This is the "brain" of the engine — all abstractions, interfaces, and data structures that
have no knowledge of how things run on a real machine.

## What belongs here

Everything that could run in a unit test or a CI pipeline without a display or audio device.

## Module Layout

- **Core/**: `Singleton`, `UUID`, `Timer`, `Hash`, `Command`, `Event`/`Observer`, `ServiceLocator`
- **Scene/**: `GameObject`, `Scene`, `SceneManager`, `Transform`, `ComponentBase`, `PersistenceLayer`, `Prefab`
- **Rendering/**: `RendererComponentBase` only — pure virtual interface, no SDL
- **Physics/**: `ICollider`, `IPhysicsSystem` — interfaces only
- **Audio/**: `ISoundClip`, `ISoundMusic`, `ISoundSystem` — interfaces only
- **Input/**: `InputUtils` (enums), `IKeyboard`, `IController`, `ActionMap` (header) — no SDL
- **Asset/**: `Asset`, `AssetMetadata`, `IAssetImporter`, `IResourceManager`, `AssetRegistry`, `SoftAssetRef`, `ResourceManager`
- **Animation/**: `SpriteAnimationClip` — pure data + timing, holds `AssetHandle`s not `Sprite`s
- **Math/**: `Rect`

## Include paths

Public headers live under `include/JREngine/` (Option A — same subfolder as the old monolith).
Consumers include as `"JREngine/Core/Singleton.h"` etc. — no path changes needed anywhere.

## Adding files

1. Header → `include/JREngine/<Subsystem>/`
2. Source → `src/<Subsystem>/`
3. Register both in `JRCore/CMakeLists.txt` (files are listed explicitly, not globbed)
4. **Verify:** `grep -r "SDL" JRCore/` must return nothing after your change

## Notes

- `FixedUPdate` (capital P) — known typo in the FSM interface, keep consistent until refactored
- `ServiceLocator.cpp` initialises all three null services (`NullSoundSystem`, `NullResourceManager`, `NullPhysicsSystem`) so the engine is always in a valid state
- `AssetRegistry` owns the data-path and is the single source of truth for `GetFullDatapath()` calls at runtime
