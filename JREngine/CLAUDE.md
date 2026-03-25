# CLAUDE.md — JRRuntime (directory: JREngine/)

SDL2 static library — the platform layer of the engine. Implements all JRCore abstractions
against real hardware: SDL gives pixels, SDL_mixer gives sound, XInput gives controllers.

**CMake target name:** `JRRuntime` (directory is still named `JREngine/` — physical rename is
a future cleanup pass).

**Links:** JRCore (PUBLIC) + SDL2 + SDL2_image + SDL2_ttf + SDL2_mixer + ImGui + XInput.

## What belongs here

Everything that requires a platform dependency. If it includes an SDL header, it lives here.
Zero-dependency abstractions live in JRCore instead.

## Module Layout

- **Scene/**: — moved to JRCore
- **Core/**: — moved to JRCore
- **Asset/**: `Texture2D`, `Sprite`, `Font`, `SpriteEditor` — SDL assets.
  `TextureImporter`, `FontImporter`, `SoundClipImporter`, `SoundMusicImporter` — SDL loaders.
  `AssetLoaderRegistry` — runtime dispatch table (typeName → LoadFunc).
  `RuntimeResourceManager` — synchronous, deterministic loader for the shipped game.
- **Input/**: `InputManager`, `SDLKeyboard`, `XBoxController` — SDL + XInput implementations
- **Physics/**: `BoxPhysicsSystem` (AABB), `RigidBody2DComponent`, `Box2DColliderComponent`
- **Rendering/**: `SDLRenderer`, `SpriteRendererComponent`, `TextRendererComponent`
- **Animation/**: `SpriteAnimatorComponent` — resolves `AssetHandle`s to `Sprite`s at render time
- **Audio/**: `SDLSoundSystem`, `SDLSoundClip`, `SDLSoundMusic`
- **JREngine.h / JREngine.cpp**: Main engine class — SDL init, game loop, fixed timestep

## Adding new components

1. Header → `include/JREngine/<Subsystem>/`
2. Source → `src/<Subsystem>/`
3. Register both in `JREngine/CMakeLists.txt` (files are listed explicitly, not globbed)
4. Derive from `ComponentBase` (JRCore)

## Adding new asset types (runtime)

1. Derive from `Asset` (JRCore) for the data class — goes in `include/JREngine/Asset/`
2. Implement `IAssetImporter` (JRCore) for loading — goes in `include/JREngine/Asset/`
3. Register the load function with `AssetLoaderRegistry` at startup

## Notes

- `FixedUPdate` (capital P) — known typo in the FSM interface, keep consistent until refactored
- Physics gravity default: 40 units/sec²
- Scene renders components in registration order via `m_RendererComponents`
- Transform uses dirty flag optimization for world-space calculations
- `AssetDatabase` and `EditorResourceManager` are **not** in JRRuntime — they live in `JREditor/`
