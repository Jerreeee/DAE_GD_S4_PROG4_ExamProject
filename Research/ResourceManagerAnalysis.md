# Resource Manager System — Deep Analysis

## Architecture Overview

The asset system is split across several classes:
- **AssetRegistry** (singleton) — stores metadata + handles, deduplicates by path
- **EditorResourceManager** — loads and caches assets, has a worker thread for async
- **ResourceManager** — static facade that delegates to ServiceLocator's active IResourceManager
- **AssetImporter** (singleton) — registry of import functions, dispatches loads by asset type
- **SoftAssetRef\<T\>** — lazy-loading wrapper that caches the loaded asset on first access

## What's Good

- Clean separation: AssetRegistry (metadata) vs EditorResourceManager (loaded assets)
- `SoftAssetRef<T>` is a clever lazy-loading wrapper — elegant for deferred/circular dependencies
- Static importer registration (`static bool s_Registered = [](){...}();`) is a nice self-registering pattern
- `FontImporter.SetSize()` decorator pattern for asset variants is well thought out
- Type safety with C++20 `IsAsset` concept
- Handle-based design decouples asset identity from memory location
- Async worker thread with condition variable and job deduplication
- Path-based deduplication prevents loading the same file twice

## Issues Found

### 1. RuntimeResourceManager is Empty
Only `EditorResourceManager` exists with an actual implementation. `RuntimeResourceManager` is declared but has no logic — the "Runtime vs Editor" distinction doesn't function yet.

### 2. SoftAssetRef is Not Thread-Safe
`m_Asset` is mutable with no synchronization. Since `EditorResourceManager` has a worker thread, two threads could race on the cached `shared_ptr`. Fix: use `std::atomic<std::shared_ptr<T>>` or a mutex.

### 3. IAssetImporter Only Requires GetMetadata()
The actual import logic is a static `ImportAsset()` function registered separately via `AssetImporter::RegisterImporter()`. The interface doesn't enforce the import contract — a virtual `Import()` method would be more consistent and self-documenting.

### 4. TileMapImporter Creates Assets Recursively During Import
It calls `AssetImporter::ImportAsset(TextureImporter(...))` inside its own import. This is fragile — recursive asset creation during an import could cause issues with threading/locking in EditorResourceManager.

### 5. No Asset Unloading
Once loaded, assets live forever in `m_LoadedAssets`. No cleanup path, no reference counting for eviction. Fine for BubbleBobble's scope but not scalable.

### 6. GetAsset() Silently Returns nullptr on Invalid Handles
No logging, no way to distinguish "invalid handle" from "not yet loaded." A debug log or separate error code would help catch bugs.

### 7. AddAsset() Doesn't Set Handle on the Asset Object
`EditorResourceManager::AddAsset()` generates a new UUID and stores the asset, but never calls `asset->SetHandle(handle)`. The Asset object doesn't know its own handle after being added through this path.

### 8. Sprite::Initialize() Blocks in a Const Method
Calls `m_SoftTextureRef.Get()` which can block waiting for an async load. This could stall the render thread at an unpredictable point. Better to initialize at load time.

### 9. No Asset Dependency Tracking
`AssetMetadata` has a comment hinting at dependencies but nothing is implemented. If Sprite needs a Texture2D, that relationship isn't declared — circular or missing loads could happen silently.

### 10. Metadata uniqueID Usage is Inconsistent
Only `FontImporter` uses it (for font sizes like "@16"). All other importers leave it empty. The pattern works but isn't enforced or documented.

## Asset Loading Flow

```
1. Create importer:        FontImporter("Fonts/Pixel_NES.otf").SetSize(20)
2. Register in registry:   AssetImporter::ImportAsset(std::move(importer)) → AssetHandle
3. Load from disk:         ResourceManager::GetAsset<Font>(handle) → AssetRef<Font>

Or lazy:
3. Wrap in soft ref:       SoftAssetRef<Font>(handle)
4. Access later:           softRef->GetSize()  // loads on first access
```

## Verdict

For a school/exam project, this is a well-architected system. The handle-based design with lazy loading via SoftAssetRef shows good understanding of resource management patterns. The main gaps (empty RuntimeResourceManager, no unloading, thread safety on SoftAssetRef) are things you'd address in a production engine but are reasonable for BubbleBobble's scope.

---

## Suggestions for Improvement

### Priority 1: Thread-Safe SoftAssetRef
`SoftAssetRef::m_Asset` is mutable with no synchronization, but `EditorResourceManager` has a worker thread. Two threads could race on the cached `shared_ptr`. Fix options:
- **Simple**: Add a `std::mutex` to `SoftAssetRef` and lock around the `Get()` check-and-cache
- **Better**: Use `std::atomic<std::shared_ptr<T>>` (C++20) for lock-free thread safety
- **Simplest**: Make all asset loading synchronous and remove the worker thread — for BubbleBobble's asset count, async loading adds complexity with no perceptible benefit

### Priority 2: Fix AddAsset() Missing SetHandle
`EditorResourceManager::AddAsset()` generates a UUID and stores the asset, but never calls `asset->SetHandle(handle)`. The asset object doesn't know its own handle. Fix: add `asset->SetHandle(newHandle)` after generating the UUID, before inserting into the map.

### Priority 3: Implement Asset Unloading
Currently assets live forever once loaded. For a game with multiple levels, this means memory grows monotonically. Options:
- **Reference counting**: Track `shared_ptr` use counts. When an asset's refcount drops to 1 (only the manager holds it), mark it as evictable
- **Scene-based unloading**: Tag assets with the scene that loaded them. On scene transition, unload assets not tagged by the new scene
- **Manual unload**: Add `UnloadAsset(handle)` to the interface for explicit control

For BubbleBobble's scope, scene-based unloading would be the most practical — tie it into the existing persistence flags system.

### Priority 4: Add Logging to GetAsset() Failures
`GetAsset()` silently returns nullptr on invalid handles. Add a debug log:
```cpp
if (it == m_LoadedAssets.end()) {
    LOG_WARNING("GetAsset: invalid handle {}", handle);
    return nullptr;
}
```
This catches bugs where assets are referenced by stale handles after unloading or mistyped paths.

### Priority 5: Implement RuntimeResourceManager
`RuntimeResourceManager` is declared but empty. For a shipping game, this should:
- Load from packed/bundled asset files instead of loose files
- Skip editor-only metadata
- Optimize for load speed over flexibility

Even a minimal implementation that just delegates to `EditorResourceManager` would validate the interface split.

### Priority 6: Add Import() to IAssetImporter Interface
Currently the actual import logic is a static `ImportAsset()` function registered separately. The `IAssetImporter` interface only requires `GetMetadata()`. Adding a virtual `Import()` method would:
- Make the interface self-documenting — the contract is clear from the base class
- Allow importers to be passed around polymorphically for the full load cycle
- Eliminate the separate static registration step

### Priority 7: Asset Dependency Tracking
`AssetMetadata` hints at dependencies but nothing is implemented. When TileMapImporter creates textures during its own import, this creates implicit dependencies. Tracking them would enable:
- Automatic loading of dependencies before the parent asset
- Detecting circular dependencies at import time
- Unloading dependency chains together

A simple approach: `AssetMetadata` stores a `std::vector<AssetHandle> dependencies` populated during import.

### Priority 8: Standardize uniqueID Usage
Only `FontImporter` uses `AssetMetadata::uniqueID` (for font sizes like "@16"). Either:
- Document the convention and enforce it in other importers that could benefit (e.g., texture scale variants)
- Remove it from `AssetMetadata` and handle font size variants differently (e.g., separate importer instances with distinct paths)

### Priority 9: Fix Sprite::Initialize() Blocking in Const
`Sprite::Initialize()` calls `m_SoftTextureRef.Get()` which can block waiting for an async load. This could stall rendering unpredictably. Fix: ensure textures are loaded before sprites reference them, or add an `IsReady()` check that skips rendering if the texture isn't loaded yet.
