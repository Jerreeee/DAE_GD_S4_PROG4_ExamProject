# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Two-project C++20 CMake solution: **JREngine** (static library game engine) and **BubbleBobble** (game executable using JREngine). Uses SDL2 ecosystem, GLM, and ImGui.

## Build Commands

```bash
# Configure (from Developer Command Prompt for VS 2022, or VS Code launched from one)
cmake --preset x64-debug
cmake --preset x64-release

# Build
cmake --build out/build/x64-debug
cmake --build out/build/x64-release

# Run
./out/build/x64-debug/bin/BubbleBobble.exe
```

In VS Code: **F7** to build, **F5** to debug, **Shift+F5** to run. Must launch VS Code from "x64 Native Tools Command Prompt for VS 2022" for the Ninja generator to find Windows SDK libs.

## Compiler Settings

All three CMakeLists.txt files set `/W4 /WX` (MSVC) / `-Wall -Wextra -Wpedantic -Werror` (GCC/Clang). Warnings are errors — unused parameters, variables, etc. will fail the build.

## Architecture

### Component-Entity System
GameObjects hold components via `AddComponent<T>()` / `GetComponent<T>()`. Components derive from `ComponentBase` with lifecycle: `Start()` → `Update(dt)` / `FixedUpdate(dt)` → `OnEnable()` / `OnDisable()`. C++20 concepts enforce type constraints.

### Scene Management & Persistence
`SceneManager` owns scenes. GameObjects have persistence flags (bitfields) that control whether they survive scene transitions. The engine defines `SceneLocal` and `SceneTemporary`; the game extends with `Global` and `Levels` in `BubbleBobble/src/EngineSetup.h`.

### Service Locator
Global access to swappable systems: SoundSystem, ResourceManager, PhysicsSystem. Access via `ServiceLocator`.

### Asset System
Handle-based (`AssetHandle` = UUID). `IAssetImporter` implementations load assets, `IResourceManager` stores them. `AssetRef<T>` (shared_ptr) for ownership, `SoftAssetRef<T>` for deferred references.

### Event System
Observer-pattern with type-safe dispatch. `HashEventID("Name")` for compile-time IDs. `EventConnection` handles lifetime. Events propagate through `IObserver` interface.

### Input System
`InputManager` (singleton) manages devices and action maps. Action maps group bindings per context (menu, gameplay). `BindCommand()` maps input states to `Command` (std::function<void()>).

### Physics
AABB collision via `BoxPhysicsSystem`. Static colliders registered by group (tilemaps), dynamic colliders per-entity. Collision layers are bitflags defined in `EngineSetup.h`.

### State Machines
`FiniteStateMachine` and `HierarchicalStateMachine` in engine. States have `OnEnter/Update/FixedUpdate/OnExit/OnEvent`. Predicate-based transitions.

## Key Conventions

- Engine public headers: `JREngine/include/JREngine/`; private impl: `JREngine/src/`
- Game code is all under `BubbleBobble/src/` with no public/private split
- Smart pointers: `unique_ptr` for ownership, `shared_ptr` for assets
- Singletons: `InputManager`, `SceneManager` (via `Singleton<T>` template)
- Naming: PascalCase for types/methods, m_ prefix for members, m_p prefix for pointer members
- Post-build copies `Data/` folder and DLLs next to the executable

## Dependencies (fetched by CMake)

SDL2 (2.30.10), SDL2_image (2.8.3), SDL2_ttf (2.22.0), SDL2_mixer (2.6.3), GLM (1.0.1), ImGui (v1.91.8-docking + ImGui-Plot). Optional: VLD for leak detection.
