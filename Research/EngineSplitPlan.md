# Engine Split Plan — JRCore / JRRuntime / JREditor

**Status:** Steps 1–5 ✅ — Split complete
**Goal:** Split the monolithic `JREngine` static library into three targets with clean separation of concerns.

---

## Target Structure

```
JRCore        (static lib)  — pure C++ + GLM only. Zero platform deps. Shared by everyone.
JRRuntime     (static lib)  — SDL2 + game loop. Links JRCore publicly.
JREditor      (exe)         — asset pipeline + editor tooling. Links JRCore + JRRuntime.
BubbleBobble  (exe)         — game code. Links JRRuntime only (gets JRCore transitively).
```

---

## The Distinction — Core vs Runtime vs Editor

**JRCore = the "what"**
Pure logic. Data structures, interfaces, abstractions. No knowledge of *how* things run on a
machine. Could compile and run headlessly — no window, no SDL, no drivers. Think: scene graph,
state machines, event system, asset handle/metadata system, type system. The engine's "brain."
Only external dependency: **GLM** (header-only math, no binaries).

**JRRuntime = the "how"**
Platform implementations. Ties Core abstractions to real hardware. SDL gives pixels,
SDL_mixer gives sound, XInput gives controller input. The game loop, renderer, concrete asset
loaders — the "here's how we actually *do* this on a real machine" layer.
Deps: JRCore (PUBLIC) + SDL2 + SDL2_image + SDL2_ttf + SDL2_mixer + ImGui + XInput + GLM.

**JREditor = the "tooling"**
Everything that belongs to the asset pipeline and editor phase only. Not shipped with the game.
`AssetDatabase` registers assets and writes the manifest. `EditorResourceManager` async-loads
assets during that phase. Future: ImGui viewport, inspector panels, scene serializer.
Deps: JRCore + JRRuntime.

**Why does JREditor link JRRuntime?**
The same reason Unreal and Unity link their runtime inside the editor: Play-in-editor requires
the full platform layer — renderer, physics, audio, input. The editor is a **superset** of the
runtime, not a separate thing. The shipped game exe (BubbleBobble) is the stripped-down version
that contains only JRRuntime and no editor tooling. The editor contains everything the runtime
has, plus the tools on top. The fact that manifest-writing also works as a side effect of this
link is a bonus, not the reason.

**Why two resource managers?**
This mirrors the Hazel Engine split. The two contexts have fundamentally different needs:

- `EditorResourceManager` — designed for a live editor. Loads assets **asynchronously** so the
  editor UI never freezes. Future: hot-reload when files change on disk, loading state tracking
  per asset (spinners in asset browser), prefetching, thumbnail generation. The editor is
  long-lived and assets come and go.
- `RuntimeResourceManager` — designed for the shipped game. **Synchronous and deterministic**.
  Load order is predictable, the manifest is fixed at cook time, no hot-reload, memory is
  managed tightly. You know exactly what assets exist and when they load.

Note: in the current codebase `EditorResourceManager` is only used during the asset registration
phase before the game loop. Once a real editor with a live viewport exists, it will be used the
entire time the editor is open — which is when the distinction becomes fully meaningful.
Keep both — the naming and separation are correct. The `EditorResourceManager` will grow into
its name as the editor grows.

**Why JRCore?**
1. **Headless asset cooking** — the manifest can be cooked in CI without a display or SDL.
2. **Unit testability** — FSMs, scene graph, events can be tested without SDL initializing.
3. **Hard boundary** — the linker enforces that game logic never accidentally pulls in SDL.
4. **Future editor viewport** — when a full editor with an embedded game viewport is added,
   game logic (BubbleBobble) can be split into `BubbleBobbleLib` linked by both the game exe
   and the editor without SDL leaking everywhere.

---

## Progress Tracker

| Step | Description | Status |
|------|-------------|--------|
| 1 | Introduce AssetLoaderRegistry, rename AssetImporter → AssetDatabase, split editor/runtime asset phases | ✅ DONE |
| 2a | Decouple GameObject.cpp from SDLRenderer | ✅ DONE |
| 2b | Decouple Box2DColliderComponent.cpp from SDLRenderer | ✅ DONE |
| 2c | Decouple JREngine.cpp from EditorResourceManager | ✅ DONE |
| 2d | Fix SpriteAnimationClip to remove Sprite/Texture2D dependency | ✅ DONE |
| 2e | Move path resolution off AssetDatabase into AssetRegistry | ✅ DONE |
| 3 | Create JRCore CMake target — extract all zero-dep files, verify standalone build | ✅ DONE |
| 4 | Rename JREngine → JRRuntime, link JRCore | ✅ DONE |
| 5 | Create JREditor exe — move AssetDatabase + EditorResourceManager, new main.cpp | ✅ DONE |

---

## Current State (after Step 5 — split complete)

### JREditor temporary coupling to BubbleBobble

`JREditor.exe` currently compiles a subset of BubbleBobble's source files directly:
- `BubbleBobble/src/Assets/AssetManifest.cpp` — calls `AssetDatabase::RegisterAll()` for every game asset
- `BubbleBobble/src/Assets/AnimsDataImporter.cpp`
- `BubbleBobble/src/Assets/LevelDataImporter.cpp`
- `BubbleBobble/src/TileMap/TileMap.cpp` + `TileMapImporter.cpp`
- `BubbleBobble/src/Assets/SceneDescriptorImporter.cpp`

**Why:** The editor needs to know which game assets to register. Until a proper project/asset system exists (where the editor discovers assets itself or reads a project file), the game's `AssetManifest` is the source of truth.

**When this changes:** Once `BubbleBobble` is split into `BubbleBobbleLib` (static lib) + thin `BubbleBobble.exe`, `JREditor` will link `BubbleBobbleLib` instead of compiling those files directly. See *Future: Full Editor Viewport* section below.

### Manifest generation — POST_BUILD automation

`JREditor.exe` is run automatically as a `POST_BUILD` step on `BubbleBobble` (see `BubbleBobble/CMakeLists.txt`). The order is:
1. Copy `Data/` folder next to the exe
2. Copy runtime DLLs next to the exe
3. Run `JREditor.exe` → writes `Data/asset_manifest.txt`

`add_dependencies(BubbleBobble JREditor)` ensures JREditor is always compiled first. The manifest is regenerated on every build that recompiles BubbleBobble. `BubbleBobble.exe` will print a clear error and exit if the manifest is missing (e.g. on a fresh checkout before the first build).

---

## Current State (after Step 1)

The codebase still lives in one `JREngine` static library. The asset phase split is done:
- `AssetDatabase` — editor-phase singleton, registers assets into AssetRegistry, writes manifest
- `AssetLoaderRegistry` — runtime dispatch table, maps typeName → LoadFunc
- `AssetRegistry` — shared persistent lookup table (editor writes, runtime reads)
- `EditorResourceManager` — async loader used during the editor/registration phase
- `RuntimeResourceManager` — synchronous loader used during gameplay

**Key file locations after Step 1:**
```
JREngine/include/JREngine/Asset/AssetDatabase.h
JREngine/src/Asset/AssetDatabase.cpp
JREngine/include/JREngine/Asset/AssetLoaderRegistry.h
JREngine/src/Asset/AssetLoaderRegistry.cpp
JREngine/include/JREngine/Asset/AssetRegistry.h
JREngine/src/Asset/AssetRegistry.cpp
JREngine/include/JREngine/Asset/EditorResourceManager.h
JREngine/src/Asset/EditorResourceManager.cpp
JREngine/src/Asset/RuntimeResourceManager.cpp
JREngine/src/JREngine.cpp                         ← still creates EditorResourceManager
```

---

## Complete File Assignment

### JRCore files

| Path (relative to JREngine/) | Notes |
|------------------------------|-------|
| `include/JREngine/Core/Singleton.h` | |
| `include/JREngine/Core/Timer.h` + `.cpp` | |
| `include/JREngine/Core/UUID.h` + `.cpp` | |
| `include/JREngine/Core/Hash.h` | |
| `include/JREngine/Core/Command.h` | |
| `include/JREngine/Core/Event.h` + `.cpp` | |
| `include/JREngine/Core/Observer.h` + `.cpp` | |
| `include/JREngine/Core/State.h` + `.cpp` | |
| `include/JREngine/Core/FiniteStateMachine.h` + `.cpp` | |
| `include/JREngine/Core/HierarchicalStateMachine.h` + `.cpp` | |
| `include/JREngine/Core/ServiceLocator.h` + `.cpp` | forward-declares all service interfaces |
| `include/JREngine/Math/Rect.h` | |
| `include/JREngine/Scene/PersistenceLayer.h` | |
| `include/JREngine/Scene/Transform.h` + `.cpp` | |
| `include/JREngine/Scene/ComponentBase.h` + `.cpp` | |
| `include/JREngine/Scene/GameObject.h` + `.cpp` | requires Step 2a fix first |
| `include/JREngine/Scene/Scene.h` + `.cpp` | |
| `include/JREngine/Scene/SceneManager.h` + `.cpp` | |
| `include/JREngine/Scene/Prefab.h` | |
| `include/JREngine/Rendering/RendererComponentBase.h` + `.cpp` | no SDL — pure virtual interface |
| `include/JREngine/Physics/ICollider.h` | |
| `include/JREngine/Physics/IPhysicsSystem.h` | |
| `include/JREngine/Audio/ISoundClip.h` | |
| `include/JREngine/Audio/ISoundMusic.h` | |
| `include/JREngine/Audio/ISoundSystem.h` | |
| `include/JREngine/Input/InputUtils.h` | enums + base interfaces, no SDL |
| `include/JREngine/Input/IKeyboard.h` | |
| `include/JREngine/Input/IController.h` | |
| `include/JREngine/Input/ActionMap.h` | header has no SDL — impl stays in Runtime |
| `include/JREngine/Asset/Asset.h` + `.cpp` | |
| `include/JREngine/Asset/AssetMetadata.h` | shared by editor + runtime |
| `include/JREngine/Asset/IAssetImporter.h` | interface — game importers inherit from it |
| `include/JREngine/Asset/IResourceManager.h` | |
| `include/JREngine/Asset/AssetRegistry.h` + `.cpp` | editor writes, runtime reads |
| `include/JREngine/Asset/SoftAssetRef.h` | |
| `include/JREngine/Asset/ResourceManager.h` | static utility wrapper |
| `include/JREngine/Animation/SpriteAnimationClip.h` + `.cpp` | requires Step 2d fix first |

**JRCore external deps: GLM only.**

---

### JRRuntime files

| Path (relative to JREngine/) | Notes |
|------------------------------|-------|
| `JREngine.h` + `src/JREngine.cpp` | requires Step 2c fix first |
| `include/JREngine/Input/InputManager.h` + `.cpp` | SDL + ImGui event handling |
| `src/Input/SDLKeyboard.h` + `.cpp` | SDL keyboard impl |
| `src/Input/XBoxController.h` + `.cpp` | XInput impl |
| `include/JREngine/Rendering/SDLRenderer.h` + `.cpp` | SDL2 renderer + ImGui |
| `include/JREngine/Rendering/SpriteRendererComponent.h` + `.cpp` | |
| `include/JREngine/Rendering/TextRendererComponent.h` + `.cpp` | |
| `include/JREngine/Animation/SpriteAnimatorComponent.h` + `.cpp` | resolves AssetHandles to Sprites |
| `include/JREngine/Audio/` SDLSoundSystem + SDLSoundClip + SDLSoundMusic (h+cpp) | SDL_mixer |
| `include/JREngine/Physics/BoxPhysicsSystem.h` + `.cpp` | |
| `include/JREngine/Physics/RigidBody2DComponent.h` + `.cpp` | |
| `include/JREngine/Physics/Box2DColliderComponent.h` + `.cpp` | requires Step 2b fix first |
| `include/JREngine/Asset/Texture2D.h` + `.cpp` | SDL_image + SDL_ttf |
| `include/JREngine/Asset/Sprite.h` + `.cpp` | |
| `include/JREngine/Asset/Font.h` + `.cpp` | SDL_ttf |
| `include/JREngine/Asset/SpriteEditor.h` + `.cpp` | |
| `include/JREngine/Asset/TextureImporter.h` + `.cpp` | |
| `include/JREngine/Asset/FontImporter.h` + `.cpp` | |
| `include/JREngine/Asset/SoundClipImporter.h` + `.cpp` | |
| `include/JREngine/Asset/SoundMusicImporter.h` + `.cpp` | |
| `include/JREngine/Asset/AssetLoaderRegistry.h` + `.cpp` | runtime dispatch table |
| `include/JREngine/Asset/RuntimeResourceManager.h` + `.cpp` | |

**JRRuntime external deps: JRCore (PUBLIC) + SDL2 + SDL2_image + SDL2_ttf + SDL2_mixer + ImGui + XInput + GLM.**

---

### JREditor files (new — created in Step 5)

| File | Notes |
|------|-------|
| `JREditor/src/main.cpp` | NEW — editor entry point |
| `include/JREngine/Asset/AssetDatabase.h` → moved to `JREditor/` | editor-phase only |
| `src/Asset/AssetDatabase.cpp` → moved to `JREditor/` | |
| `include/JREngine/Asset/EditorResourceManager.h` → moved to `JREditor/` | |
| `src/Asset/EditorResourceManager.cpp` → moved to `JREditor/` | |

**JREditor external deps: JRCore + JRRuntime (transitively gets SDL).**

---

## Step 2 — Prerequisite Code Fixes

Five coupling points must be broken before the CMake split can happen.
**Do all of these before touching CMakeLists.txt.**

---

### Step 2a — Decouple `GameObject.cpp` from `SDLRenderer`

**Problem:** `GameObject.cpp` calls `SDLRenderer::GetInstance().RemoveRendererComponent(this)`
when destroyed. A game object must not know about the concrete renderer — it would drag
`SDLRenderer.h` (SDL dep) into `GameObject.cpp`, which must be JRCore.

**Fix:** Move renderer component cleanup responsibility to `Scene`.
- `Scene` already owns the `m_RendererComponents` list.
- When `Scene::RemoveGameObject(go)` runs, iterate and remove any renderer components
  that belong to that game object before removing the game object itself.
- Delete the `SDLRenderer` call from `GameObject.cpp` entirely.
- Remove `#include "Rendering/SDLRenderer.h"` from `GameObject.cpp`.

**Build & Commit:**
```bash
cmake --build out/build/x64-debug
./out/build/x64-debug/bin/BubbleBobble.exe   # smoke test
git add -p
git commit -m "Decouple GameObject from SDLRenderer — cleanup via Scene"
git push
```

---

### Step 2b — Decouple `Box2DColliderComponent.cpp` from `SDLRenderer`

**Problem:** `Box2DColliderComponent.cpp` includes `SDLRenderer.h` to draw debug boxes.
This drags a Runtime dep into what should be a Core physics component.

**Fix:** Delete the SDL debug draw code entirely from `Box2DColliderComponent.cpp`.
Remove the `#include "SDLRenderer.h"` and all debug rendering calls.
No ifdef, no workaround — just delete it.

**Build & Commit:**
```bash
cmake --build out/build/x64-debug
./out/build/x64-debug/bin/BubbleBobble.exe   # smoke test
git add -p
git commit -m "Decouple Box2DColliderComponent from SDLRenderer — remove debug draw"
git push
```

---

### Step 2c — Decouple `JREngine.cpp` from `EditorResourceManager`

**Problem:** `JREngine.cpp` (JRRuntime) currently creates `EditorResourceManager` in its
constructor and switches resource managers inside `Run()`. EditorResourceManager must move to
JREditor, so JRRuntime cannot create it.

**Fix:** Remove resource manager management from JREngine entirely.
The **caller** is responsible for registering the correct resource manager via
`ServiceLocator::RegisterResourceManager()` before calling `JREngine::Run()`.

```cpp
// BEFORE (JREngine constructor):
ServiceLocator::RegisterResourceManager(std::make_unique<EditorResourceManager>());
auto erm = static_cast<EditorResourceManager*>(&ServiceLocator::GetResourceManager());
erm->Init();

// AFTER (JREngine constructor):
// Nothing — caller registers their resource manager before constructing JREngine
```

```cpp
// BEFORE (JREngine::Run()):
registerAssets();
auto manifestPath = AssetDatabase::GetInstance().GetDatapath() / "asset_manifest.txt";
AssetDatabase::GetInstance().SerializeManifest(manifestPath);
ServiceLocator::RegisterResourceManager(std::make_unique<RuntimeResourceManager>(manifestPath));
buildScenes();
// game loop...

// AFTER (JREngine::Run()):
// No resource manager switching — caller already set up RuntimeResourceManager.
// Run() just calls buildScenes() then enters the game loop.
buildScenes();
// game loop...
```

**New caller flow in JREditor's `main.cpp`:**
```cpp
// 1. Init engine (SDL window, renderer, audio — no resource manager)
JREngine engine(dataPath);

// 2. Editor phase: register assets, write manifest
ServiceLocator::RegisterResourceManager(std::make_unique<EditorResourceManager>());
static_cast<EditorResourceManager*>(&ServiceLocator::GetResourceManager())->Init();
AssetManifest::RegisterAll();
auto manifestPath = AssetDatabase::GetInstance().GetDatapath() / "asset_manifest.txt";
AssetDatabase::GetInstance().SerializeManifest(manifestPath);

// 3. Switch to runtime resource manager
ServiceLocator::RegisterResourceManager(
    std::make_unique<RuntimeResourceManager>(manifestPath));

// 4. Run game loop
engine.Run([]{ BuildScenes(); });
```

**Also:** Remove `#include "Asset/EditorResourceManager.h"` and
`#include "Asset/AssetDatabase.h"` from `JREngine.cpp`.
JREngine.cpp should have zero knowledge of either.

**Build & Commit:**
```bash
cmake --build out/build/x64-debug
./out/build/x64-debug/bin/BubbleBobble.exe   # smoke test — game must still run with assets loading
git add -p
git commit -m "Decouple JREngine from EditorResourceManager — caller owns resource manager lifecycle"
git push
```

---

### Step 2d — Fix `SpriteAnimationClip` to remove `Sprite`/`Texture2D` dependency

**Problem:** `SpriteAnimationClip.h` holds `SoftAssetRef<Sprite>` per frame.
`Sprite` → `Texture2D` → `SDL.h`. This prevents SpriteAnimationClip from living in JRCore.

**Fix:** Replace per-frame `SoftAssetRef<Sprite>` with `AssetHandle`.
SpriteAnimationClip becomes pure data + timing — no SDL anywhere.
`SpriteAnimatorComponent` (JRRuntime) resolves handles to Sprites via `ResourceManager`
at render time.

```cpp
// BEFORE (SpriteAnimationClip.h):
#include "JREngine/Asset/SoftAssetRef.h"
#include "JREngine/Asset/Sprite.h"
// ...
struct Frame { SoftAssetRef<Sprite> sprite; float duration; };
std::vector<Frame> m_Frames;

// AFTER (SpriteAnimationClip.h):
#include "JREngine/Asset/Asset.h"   // for AssetHandle
// ...
struct Frame { AssetHandle spriteHandle; float duration; };
std::vector<Frame> m_Frames;
```

`SpriteAnimatorComponent::Update()` resolves the handle each frame:
```cpp
auto sprite = ResourceManager::GetAsset<Sprite>(clip->GetCurrentFrame().spriteHandle);
m_SpriteRenderer->SetSprite(sprite);
```

**Build & Commit:**
```bash
cmake --build out/build/x64-debug
./out/build/x64-debug/bin/BubbleBobble.exe   # smoke test — animations must still play
git add -p
git commit -m "SpriteAnimationClip: replace SoftAssetRef<Sprite> with AssetHandle — removes SDL dep"
git push
```

---

### Step 2e — Move path resolution off `AssetDatabase` into `AssetRegistry`

**Problem:** `AssetDatabase::GetFullDatapath()` is called by importer `Load()` functions in
JRRuntime (TextureImporter, FontImporter, SDLSoundSystem, etc.) and in BubbleBobble importers.
If AssetDatabase moves to JREditor, those JRRuntime files would depend on JREditor — circular.

**Fix:** Move `SetDataPath()` + `GetFullDatapath()` from `AssetDatabase` to `AssetRegistry`
(which lives in JRCore and is already visible to everyone).

```cpp
// Add to AssetRegistry.h:
void SetDataPath(const std::filesystem::path& dataPath);
std::filesystem::path GetFullDatapath(const std::filesystem::path& relativePath) const;

private:
    std::filesystem::path m_DataPath{};
```

Update all call sites:
```cpp
// BEFORE (all importers + SDLSoundSystem):
AssetDatabase::GetInstance().GetFullDatapath(metadata.filepath)

// AFTER:
AssetRegistry::GetInstance().GetFullDatapath(metadata.filepath)
```

AssetDatabase::Init() becomes just:
```cpp
void AssetDatabase::Init(const std::filesystem::path& dataPath)
{
    AssetRegistry::GetInstance().SetDataPath(dataPath);
}
```

After this, `AssetDatabase.h` only needs: `#include "JREngine/Asset/IAssetImporter.h"` — no
`<filesystem>` path members at all.

**Build & Commit:**
```bash
cmake --build out/build/x64-debug
./out/build/x64-debug/bin/BubbleBobble.exe   # smoke test — all assets must still load
git add -p
git commit -m "Move datapath resolution from AssetDatabase to AssetRegistry"
git push
```

---

## Step 3 — Create JRCore CMake target

Create a new top-level directory `JRCore/` with its own `CMakeLists.txt`.

**Directory structure:**
```
JRCore/
  CMakeLists.txt
  include/JRCore/         ← public headers (copy/move from JREngine/include/JREngine/)
    Core/
    Math/
    Scene/
    Rendering/RendererComponentBase.h
    Physics/ICollider.h, IPhysicsSystem.h
    Audio/ISoundClip.h, ISoundMusic.h, ISoundSystem.h
    Input/InputUtils.h, IKeyboard.h, IController.h, ActionMap.h
    Asset/Asset.h, AssetMetadata.h, IAssetImporter.h, IResourceManager.h,
          AssetRegistry.h, SoftAssetRef.h, ResourceManager.h
    Animation/SpriteAnimationClip.h
  src/                    ← implementation files (copy/move from JREngine/src/)
    Core/
    Scene/
    Rendering/RendererComponentBase.cpp
    Asset/AssetRegistry.cpp
    Animation/SpriteAnimationClip.cpp
```

**`JRCore/CMakeLists.txt` skeleton:**
```cmake
set(TARGET_NAME JRCore)

set(SOURCE_FILES
    "Core/Timer.cpp"
    "Core/UUID.cpp"
    "Core/Event.cpp"
    "Core/Observer.cpp"
    "Core/State.cpp"
    "Core/FiniteStateMachine.cpp"
    "Core/HierarchicalStateMachine.cpp"
    "Core/ServiceLocator.cpp"
    "Scene/Transform.cpp"
    "Scene/ComponentBase.cpp"
    "Scene/GameObject.cpp"
    "Scene/Scene.cpp"
    "Scene/SceneManager.cpp"
    "Rendering/RendererComponentBase.cpp"
    "Asset/Asset.cpp"
    "Asset/AssetRegistry.cpp"
    "Animation/SpriteAnimationClip.cpp"
)
AddPrefix(SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/")

# ... INCLUDE_FILES list mirrors the headers above

add_library(${TARGET_NAME} STATIC ${SOURCE_FILES} ${INCLUDE_FILES})
target_include_directories(${TARGET_NAME}
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/JRCore"
            "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
target_link_libraries(${TARGET_NAME} PUBLIC glm::glm)
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)
target_compile_options(${TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)
```

**Include path change for consumers:**
JRCore headers will be under `include/JRCore/` so includes become `"JRCore/Core/Singleton.h"`.
JRRuntime and BubbleBobble currently use `"JREngine/Core/Singleton.h"`.
Two options:
- **Option A (simple):** Keep public include dir as `include/` with subfolder `JREngine/` — headers
  stay at the same paths. JRRuntime adds `JRCore`'s include dir and nothing changes in source files.
- **Option B (clean):** Rename include subfolder to `JRCore/`. Update all includes in JRRuntime
  and BubbleBobble to use `"JRCore/..."` for Core headers and `"JRRuntime/..."` for Runtime headers.

**Recommendation: Option A first** (zero source changes). Can rename later as a cleanup pass.

**Build verification & Commit:**
```bash
cmake --build out/build/x64-debug --target JRCore
grep -r "SDL" JRCore/   # must return nothing
cmake --build out/build/x64-debug   # full build must still pass
./out/build/x64-debug/bin/BubbleBobble.exe   # smoke test
git add -p
git commit -m "Step 3: introduce JRCore static lib — extract zero-dep engine files"
git push
```

---

## Step 4 — Rename JREngine → JRRuntime

This is mostly a CMake rename. Source files stay in their current locations during the transition
(can be physically moved in a follow-up cleanup).

**Changes:**
1. Rename `JREngine/CMakeLists.txt` target from `JREngine` to `JRRuntime`.
2. Remove all files listed in the JRCore table from JRRuntime's source/include lists.
3. Add `target_link_libraries(JRRuntime PUBLIC JRCore)`.
4. In root `CMakeLists.txt`: add `add_subdirectory(JRCore)` before `add_subdirectory(JREngine)`.
5. In `BubbleBobble/CMakeLists.txt`: change `target_link_libraries(BubbleBobble JREngine)`
   to `target_link_libraries(BubbleBobble JRRuntime)`.

**Build verification & Commit:**
```bash
cmake --build out/build/x64-debug
./out/build/x64-debug/bin/BubbleBobble.exe   # game must run identically
git add -p
git commit -m "Step 4: rename JREngine → JRRuntime, link JRCore"
git push
```

---

## Step 5 — Create JREditor exe

Create `JREditor/` with its own `CMakeLists.txt`.

**Move these files from JREngine/ to JREditor/:**
- `JREngine/include/JREngine/Asset/AssetDatabase.h` → `JREditor/include/AssetDatabase.h`
- `JREngine/src/Asset/AssetDatabase.cpp` → `JREditor/src/AssetDatabase.cpp`
- `JREngine/include/JREngine/Asset/EditorResourceManager.h` → `JREditor/include/EditorResourceManager.h`
- `JREngine/src/Asset/EditorResourceManager.cpp` → `JREditor/src/EditorResourceManager.cpp`

**Create new:**
- `JREditor/src/main.cpp` — editor entry point (see flow in Step 2c above)

**`JREditor/CMakeLists.txt` skeleton:**
```cmake
set(TARGET_NAME JREditor)

set(SOURCE_FILES
    "src/main.cpp"
    "src/AssetDatabase.cpp"
    "src/EditorResourceManager.cpp"
)
AddPrefix(SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/")

add_executable(${TARGET_NAME} ${SOURCE_FILES})
target_include_directories(${TARGET_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(${TARGET_NAME} PRIVATE JRRuntime)  # transitively gets JRCore

target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)
target_compile_options(${TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)
```

**BubbleBobble after Step 5:**
BubbleBobble still needs to run standalone during development (not just through JREditor).
Its `Main.cpp` needs `AssetDatabase` and `EditorResourceManager` to run the editor phase.

**Solution: BubbleBobble links JREditor.**
```cmake
# BubbleBobble/CMakeLists.txt
target_link_libraries(BubbleBobble PRIVATE JRRuntime JREditor)
```
This gives BubbleBobble access to `AssetDatabase` and `EditorResourceManager` so its
`Main.cpp` can run the full flow (register assets → cook manifest → switch to
RuntimeResourceManager → run game loop) exactly as before.

When shipping, BubbleBobble would be rebuilt to skip the editor phase entirely and load
from a pre-cooked manifest — but that is a future concern, not needed now.

**Build verification & Commit:**
```bash
cmake --build out/build/x64-debug --target JREditor
cmake --build out/build/x64-debug --target BubbleBobble
grep -r "#include.*SDL" JRCore/src/   # must return nothing
./out/build/x64-debug/bin/BubbleBobble.exe   # smoke test
git add -p
git commit -m "Step 5: create JREditor exe — move AssetDatabase + EditorResourceManager out of JRRuntime"
git push
```

---

## Future: Full Editor Viewport

When a visual editor with an embedded game viewport is added, the path is:
1. Split `BubbleBobble` into `BubbleBobbleLib` (static lib: all game logic, scene builders,
   components) + `BubbleBobble.exe` (thin launcher that calls `BubbleBobbleLib::Run(engine)`).
2. `JREditor.exe` links `JRRuntime + BubbleBobbleLib`.
3. The editor embeds a JRRuntime viewport as an SDL texture rendered into an ImGui image panel.
4. The editor calls `SceneManager`, `InputManager` etc. from JRRuntime directly.
5. Nothing about the JRCore/JRRuntime/JREditor split changes — just add `BubbleBobbleLib`.

The current split already supports this future cleanly.

---

## Quick Reference: What Goes Where

| Class / File | Target |
|---|---|
| Singleton, Timer, UUID, Event, Observer, FSM, HSM, ServiceLocator | JRCore |
| Scene, GameObject, Transform, ComponentBase, SceneManager | JRCore |
| RendererComponentBase | JRCore (no SDL — pure virtual) |
| ICollider, IPhysicsSystem | JRCore |
| ISoundClip, ISoundMusic, ISoundSystem | JRCore |
| InputUtils, IKeyboard, IController, ActionMap (header) | JRCore |
| Asset, AssetMetadata, IAssetImporter, IResourceManager | JRCore |
| AssetRegistry, SoftAssetRef, ResourceManager | JRCore |
| SpriteAnimationClip (after fix) | JRCore |
| AssetLoaderRegistry | JRRuntime |
| RuntimeResourceManager | JRRuntime |
| SDLRenderer, SpriteRendererComponent, TextRendererComponent | JRRuntime |
| SpriteAnimatorComponent | JRRuntime |
| SDLSoundSystem, SDLSoundClip, SDLSoundMusic | JRRuntime |
| BoxPhysicsSystem, RigidBody2DComponent, Box2DColliderComponent | JRRuntime |
| Texture2D, Sprite, Font, SpriteEditor | JRRuntime |
| TextureImporter, FontImporter, SoundClipImporter, SoundMusicImporter | JRRuntime |
| InputManager, SDLKeyboard, XBoxController | JRRuntime |
| JREngine (main loop) | JRRuntime |
| AssetDatabase | JREditor |
| EditorResourceManager | JREditor |
