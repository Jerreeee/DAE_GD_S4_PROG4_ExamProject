# JREngine

**Name:** Denayer Jeroen  
**Class:** 2DAE10  
**[GitHub Repository](https://github.com/Jerreeee/DAE_GD_S4_PROG4_ExamProject)**

## Table of Contents

- [Engine Features](#engine-features)
  - [Resource Manager](#resource-manager)
    - [Generic Asset Storage](#-generic-asset-storage)
    - [Asset Registry](#-asset-registry)
    - [Lazy Loading](#-lazy-loading)
  - [Persistence](#persistence)
    - [Built-in Layers](#built-in-layers)
    - [Custom Game Layers](#custom-game-layers)
  - [Input System](#input-system)
  - [Box Physics System](#box-physics-system)

---

## Engine Features

---

### Resource Manager

#### 🔹 Generic Asset Storage

```cpp
std::map<AssetHandle, AssetRef<Asset>> m_LoadedAssets{};
```

Assets are managed as `AssetRef<Asset>` (internally a `std::shared_ptr<T>`), allowing the engine to expose a single `GetAsset()` method for any asset type, instead of separate methods like `GetTexture2D()` or `GetFont()`.

#### 🔹 Asset Registry

```cpp
AssetHandle ImportAsset(IAssetImporter&& importer);
std::map<AssetHandle, AssetMetadata> m_AssetHandleToMetadata{};
```

The first time an asset is imported, it’s registered and assigned a handle. The handle can then be used to load the asset. Each asset type has its own `AssetImporter` class to handle loading logic.

#### 🔹 Lazy Loading

Each `AssetImporter` can specify whether its asset type supports async loading. If so, the engine loads the asset in the background when appropriate. To support early referencing of assets that aren’t yet loaded, I implemented `SoftAssetRef<T>` (inspired by Unreal’s `TSoftObjectPtr<T>`).

---

### Persistence

Each `GameObject` and `Scene` has a persistence flag:

```cpp
uint32_t m_PersistenceScope{ PersistenceLayer::SceneLocal };
```

#### Built-in Layers

```cpp
struct PersistenceLayer
{
    static constexpr uint32_t SceneLocal = 1 << 0;     // Stays in current scene
    static constexpr uint32_t SceneTemporary = 1 << 1; // Removed on scene switch
};
```

#### Custom Game Layers

```cpp
struct PersistenceLayer
{
    static constexpr uint32_t SceneLocal = JRE::PersistenceLayer::SceneLocal;
    static constexpr uint32_t SceneTemporary = JRE::PersistenceLayer::SceneTemporary;
    static constexpr uint32_t Global = 1 << 2;
    static constexpr uint32_t Levels = 1 << 3;
};
```

Scene transitions handle persistence by comparing scene and game object flags. A similar bitflag system is used for collision layers (like Unity’s collision matrix).

---

### Input System

The `InputSystem` maintains a collection of input devices and input contexts using action maps.

```cpp
std::vector<DeviceInfo> m_Devices{};
std::vector<ActionMap> m_ActionMaps{};
Each ActionMap defines its own set of bindings and specifies which devices it applies to. You can enable or disable individual action maps to create flexible input contexts. Only the active (enabled) action maps are processed, making it easy to switch between control schemes depending on the game state (e.g., menu, inGame, ...).

---

### Box Physics System

A dedicated `BoxPhysicsSystem` handles physics for `Box2D`-style collision shapes. It’s registered as a service, so it can be swapped out or replaced if needed.

- `Box2DColliderComponent` automatically registers/unregisters with the physics system.
- Static colliders (like for tilemaps) are managed separately for performance.
- Designed to allow easy custom physics systems in other games built on the engine.