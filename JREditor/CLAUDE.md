# CLAUDE.md — JREditor

Standalone executable. Runs the **editor/asset-registration phase**: registers all game assets
into `AssetRegistry`, serialises the result to `Data/asset_manifest.txt`, then exits.
`BubbleBobble.exe` reads that manifest at startup via `RuntimeResourceManager`.

**Links:** JRRuntime (privately) → gets JRCore transitively. No game-loop, no SDL window opened
for rendering — JREngine initialises SDL subsystems but the process exits before `Run()`.

## Entry point

`src/main.cpp` — full flow:
1. Locate `Data/` folder (`./Data/` or `../Data/`)
2. `AssetDatabase::Init(dataPath)` — sets the data-path in `AssetRegistry`
3. Register `EditorResourceManager` with `ServiceLocator`
4. `BubbleBobble::AssetManifest::RegisterAll()` — registers every game asset
5. `AssetDatabase::SerializeManifest(...)` — writes `asset_manifest.txt`
6. Print confirmation and exit

## Editor-phase classes

- `AssetDatabase` (`include/AssetDatabase.h`, `src/AssetDatabase.cpp`) — editor-only singleton.
  Wraps `AssetRegistry::Register()` and `Serialize()`. Has no runtime counterpart.
- `EditorResourceManager` (`include/EditorResourceManager.h`, `src/EditorResourceManager.cpp`) —
  async asset loader used during registration. Spins up a worker thread, loads assets in the
  background. Designed for a future live editor that needs non-blocking loads.

## Temporary coupling to BubbleBobble

`JREditor` currently compiles the following BubbleBobble source files directly:

| File | Reason |
|------|--------|
| `BubbleBobble/src/Assets/AssetManifest.cpp` | Calls `RegisterAll()` — the game's asset list |
| `BubbleBobble/src/Assets/AnimsDataImporter.cpp` | Game-specific importer |
| `BubbleBobble/src/Assets/LevelDataImporter.cpp` | Game-specific importer |
| `BubbleBobble/src/TileMap/TileMap.cpp` | Data class used by TileMapImporter |
| `BubbleBobble/src/TileMap/TileMapImporter.cpp` | Game-specific importer |
| `BubbleBobble/src/Assets/SceneDescriptorImporter.cpp` | Game-specific importer |

**Why:** The editor has no project-file system yet. The game's `AssetManifest` is the only
source of truth for which assets exist.

**When this goes away:** Once `BubbleBobble` is split into `BubbleBobbleLib` (static lib) +
thin `BubbleBobble.exe`, `JREditor` will link `BubbleBobbleLib` instead of compiling those
files directly. See `Research/EngineSplitPlan.md` — *Future: Full Editor Viewport*.

## Build automation

`JREditor.exe` is run automatically as a CMake `POST_BUILD` step on `BubbleBobble`
(declared in `BubbleBobble/CMakeLists.txt`). Execution order within that POST_BUILD:

1. Copy `Data/` folder next to the exe — so JREditor can find asset files
2. Copy runtime DLLs
3. **Run JREditor.exe** → writes `Data/asset_manifest.txt`

`add_dependencies(BubbleBobble JREditor)` guarantees JREditor is compiled before
BubbleBobble links. The manifest is regenerated on every build that recompiles BubbleBobble.
