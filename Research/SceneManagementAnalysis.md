# Scene Management System Analysis

## Overview

The Scene Management system is the backbone of JREngine's runtime. It manages the lifecycle of **Scenes**, **GameObjects**, **Components**, and **Transforms**, and handles object persistence across scene transitions via a bitfield system.

**Key files:**
- `JREngine/include/JREngine/Scene/SceneManager.h` / `src/Scene/SceneManager.cpp`
- `JREngine/include/JREngine/Scene/Scene.h` / `src/Scene/Scene.cpp`
- `JREngine/include/JREngine/Scene/GameObject.h` / `src/Scene/GameObject.cpp`
- `JREngine/include/JREngine/Scene/Transform.h` / `src/Scene/Transform.cpp`
- `JREngine/include/JREngine/Scene/ComponentBase.h` / `src/Scene/ComponentBase.cpp`
- `JREngine/include/JREngine/Scene/PersistenceLayer.h`

---

## 1. SceneManager (Singleton)

Owns all scenes in a `std::map<std::string, std::unique_ptr<Scene>>`. Handles creation, switching, and persistence transfers.

### Key API

| Method | Purpose |
|--------|---------|
| `CreateScene(name, persistenceScope)` | Create and store a new scene |
| `SetNextScene(name, loadCallback)` | Queue a scene transition (deferred if mid-update) |
| `Update()` / `FixedUpdate()` | Delegate to current scene |
| `Cleanup()` | Remove destroyed objects from current scene |
| `HasScene(name)` / `GetCurrentScene()` | Queries |

### Scene Transition Flow

```
SetNextScene("Level1", callback)
  │
  ├─ If mid-update → set m_LoadNewScene flag, defer
  └─ Else → LoadNewScene() immediately
       │
       ├─ EnsureObjectPersistence(oldScene, newScene)
       │    → Transfer objects whose persistence scope matches new scene
       │    → Destroy objects that don't match
       │
       ├─ oldScene.Cleanup() + SetActive(false)
       ├─ Execute loadCallback(newScene)
       └─ newScene.SetActive(true)
```

**Key design:** Transitions are deferred when called during `Update()` or `FixedUpdate()` to prevent iterator invalidation.

---

## 2. Scene

Container for GameObjects. Manages their lifecycle, deferred addition, and renderer registration.

### Data Structures

```cpp
std::vector<std::unique_ptr<GameObject>> m_Objects;        // Active objects
std::vector<std::unique_ptr<GameObject>> m_PendingObjects;  // Added during Update()
std::vector<RendererComponentBase*>      m_RendererComponents; // Render order (non-owning)
uint32_t m_PersistenceScope;                                // Accepted persistence layers
```

### Deferred Addition Pattern

Objects added via `Add()` during `Update()` go into `m_PendingObjects`. After iteration, `MergePendingObjects()` moves them to `m_Objects`. This prevents iterator invalidation.

### Renderer Registration

When a GameObject is added, the scene scans all its components for `RendererComponentBase*` (via `dynamic_cast`) and stores them in `m_RendererComponents`. Rendering happens in **insertion order** — z-ordering is controlled by add order, not spatial position.

### SetActive Cascade

`Scene::SetActive(bool)` only activates **root objects** (no parent). Activation cascades down the parent-child hierarchy via `GameObject::UpdateActiveInHierarchy()`.

---

## 3. GameObject

The core entity. Holds components, manages parent-child relationships, and handles position via dirty-flag optimization.

### Component Management

```cpp
template<DerivedFromComponentBase T, typename... Args>
T* AddComponent(Args&&... args);   // Create + store, return raw ptr

template<DerivedFromComponentBase T>
T* GetComponent();                 // Find by type (dynamic_cast, O(n))

template<DerivedFromComponentBase T>
bool HasComponent();

template<DerivedFromComponentBase T>
void RemoveComponent();            // Mark destroyed, removed in Cleanup()
```

C++20 concept `DerivedFromComponentBase` enforces type safety at compile time.

### Lifecycle

```
Created (inactive, m_ExecutedStart = false)
  │
  ▼ Scene::SetActive(true) or SetParent()
UpdateActiveInHierarchy()
  │
  ├─ m_IsActiveInHierarchy = m_IsActive && parent.IsActiveInHierarchy()
  ├─ If newly active → Start() [once] + OnEnable() on all components
  └─ Cascades to children
  │
  ▼ Each frame
Update()  → all enabled components
FixedUpdate() → all enabled components
  │
  ▼ Destroy() called
m_IsDestroyed = true, cascades to children
  │
  ▼ Scene::Cleanup()
OnDisable() on destroyed components → erase them
```

**Important:** `Start()` is deferred until the GameObject first becomes active in the hierarchy, not at creation time.

### Parent-Child Hierarchy

```cpp
void SetParent(GameObject* parent, bool keepWorldPosition = false);
```

- Circular relationships prevented (checks `IsChild` recursively)
- `keepWorldPosition`: adjusts local position to preserve world position
- Hierarchy changes trigger `UpdateActiveInHierarchy()` cascade

### Dirty Flag Position System

```cpp
Transform m_LocalTransform;              // Position relative to parent
mutable Transform m_WorldTransform;      // Cached world position
mutable bool m_PositionIsDirty;          // Recalculation needed?
```

- `SetLocalPosition()` → marks self + all children dirty
- `GetWorldPosition()` → lazy recalculation: `world = local + parent.world`
- Only recalculates when accessed and dirty — efficient for deep hierarchies

---

## 4. Transform

Minimal — stores only `glm::vec3` position. No rotation or scale.

```cpp
class Transform {
    glm::vec3 m_Position;
public:
    const glm::vec3& GetPosition() const;
    void SetPosition(float x, float y, float z);
    void SetPosition(const glm::vec3& position);
};
```

All hierarchy math (world/local conversion, dirty flags) lives in `GameObject`, not `Transform`.

---

## 5. PersistenceLayer (Bitfield System)

Controls which objects survive scene transitions.

### Engine Layers
```cpp
static constexpr uint32_t SceneLocal    = 1 << 0;  // Dies on transition
static constexpr uint32_t SceneTemporary = 1 << 1;  // Dies on transition
```

### Game Layers (BubbleBobble — EngineSetup.h)
```cpp
static constexpr uint32_t Global = 1 << 2;  // Persists everywhere
static constexpr uint32_t Levels = 1 << 3;  // Persists between levels
```

### Masks
```cpp
PersistenceMask::MenuScene   = Global | SceneLocal
PersistenceMask::LevelScene  = Global | Levels
PersistenceMask::PlayerAndUI = Levels
```

### Transfer Logic

```cpp
uint32_t shared = object.scope & newScene.scope;
bool survives = shared & ~SceneLocal;  // Must share non-SceneLocal bits

if (survives) → transfer to new scene
else          → destroy
```

### Example

| Object | Scope | MainMenu→Level1 | Level1→Level2 |
|--------|-------|-----------------|---------------|
| GameManager | Global | Survives | Survives |
| Player | Levels | Destroyed | Survives |
| Menu Button | SceneLocal | Destroyed | N/A |
| Enemy | SceneLocal | N/A | Destroyed |

---

## 6. ComponentBase

Abstract base with virtual lifecycle hooks:

```cpp
virtual void Start();           // Once, when active
virtual void OnEnable();        // When enabled/activated
virtual void Update() = 0;     // Every frame (pure virtual)
virtual void FixedUpdate() {}   // Fixed timestep (optional)
virtual void OnDisable();       // When disabled/deactivated
```

Components can be individually enabled/disabled via `SetEnabled(bool)`, independent of GameObject active state.

---

## 7. Complete Frame Flow

```
RunOneFrame()
  │
  ├─ InputManager::ProcessInput()
  │
  ├─ While (lag >= fixedTimeStep):
  │    SceneManager::FixedUpdate() → Scene → GameObjects → Components
  │    PhysicsSystem::Update()
  │
  ├─ SceneManager::Update()
  │    ├─ Check m_LoadNewScene → LoadNewScene() if pending
  │    └─ Scene::Update()
  │         ├─ MergePendingObjects()
  │         └─ GameObjects → Components::Update()
  │
  ├─ SceneManager::Cleanup()
  │    └─ Scene::Cleanup() → remove destroyed components/objects
  │
  └─ SDLRenderer::Render()
       └─ Iterate scene.GetRenderComponents() in order
```

---

## 8. Design Patterns Summary

| Pattern | Where | Purpose |
|---------|-------|---------|
| **Singleton** | SceneManager | Global access point |
| **Deferred Operations** | Scene add, scene transitions | Prevents iterator invalidation |
| **Dirty Flag** | GameObject world position | Lazy recalculation optimization |
| **Bitfield Flags** | PersistenceLayer | Flexible object survival rules |
| **Entity-Component** | GameObject + ComponentBase | Composition over inheritance |
| **Lazy Initialization** | Start() on first activate | Avoids initializing inactive objects |

---

## 9. Suggested Improvements

### High Priority

**1. Component Lookup by Type Map — O(1) instead of O(n)**

Currently `GetComponent<T>()` iterates all components with `dynamic_cast`. For objects with many components, this is slow. Add a type-indexed cache:

```cpp
// In GameObject
std::unordered_map<std::type_index, ComponentBase*> m_ComponentTypeMap;

template<DerivedFromComponentBase T>
T* GetComponent() {
    auto it = m_ComponentTypeMap.find(std::type_index(typeid(T)));
    return it != m_ComponentTypeMap.end() ? static_cast<T*>(it->second) : nullptr;
}
```

Populate in `AddComponent()`, remove in `RemoveComponent()`. Eliminates dynamic_cast per lookup.

**2. Destroyed GameObject Removal in Cleanup()**

Currently `Scene::Cleanup()` handles destroyed components but doesn't erase destroyed GameObjects from `m_Objects`. Destroyed objects accumulate until a scene transition. Add:

```cpp
// In Scene::Cleanup()
std::erase_if(m_Objects, [](const auto& obj) { return obj->IsDestroyed(); });
```

**3. Start() Should Respect Component IsEnabled()**

There's a TODO in `GameObject.cpp` about this. Currently `Start()` is called on all components regardless of enabled state. Should skip disabled components:

```cpp
void GameObject::Start() {
    for (auto& comp : m_Components) {
        if (comp->IsEnabled())
            comp->Start();
    }
    m_ExecutedStart = true;
}
```

### Medium Priority

**4. Add Rotation and Scale to Transform**

The current Transform is position-only. For a 2D engine, at minimum add rotation (float) and scale (vec2). This would require:
- Updating dirty flag propagation to include rotation/scale
- Using a 3x3 or 4x4 matrix for world transform computation
- Updating renderers to use the full transform

```cpp
class Transform {
    glm::vec3 m_Position;
    float m_Rotation = 0.f;        // Degrees
    glm::vec2 m_Scale{1.f, 1.f};
};
```

**5. Scene Transition Events**

No events fire on scene transitions. Components can't react to "about to unload" or "just loaded". Add:

```cpp
// Broadcast before/after transition
EventManager::Dispatch(HashEventID("SceneUnloading"), oldSceneName);
EventManager::Dispatch(HashEventID("SceneLoaded"), newSceneName);
```

Or add `OnSceneUnload()` / `OnSceneLoad()` to ComponentBase.

**6. Named Renderer Layers / Z-Order**

Currently render order = insertion order. This works but is fragile. Consider an explicit z-order or named render layers:

```cpp
// Option A: Z-order on RendererComponentBase
int m_RenderOrder = 0;  // Lower = drawn first

// Option B: Named layers
enum class RenderLayer { Background, Default, UI, Overlay };
```

Sort `m_RendererComponents` by layer/order during registration or before render.

### Low Priority

**7. Scene Preloading / Async Loading**

Currently scene loading is synchronous and blocks the frame. For large scenes, consider:
- Loading scene assets in a background thread
- Showing a loading screen during transition
- `SceneManager::PreloadScene(name)` that prepares assets without activating

**8. GameObject Tagging / Querying**

No way to find GameObjects by tag or type across a scene. Add:

```cpp
// Tag system
void GameObject::SetTag(const std::string& tag);
std::vector<GameObject*> Scene::FindByTag(const std::string& tag);

// Or a faster approach with a multimap
std::unordered_multimap<std::string, GameObject*> m_TaggedObjects;
```

**9. Object Pooling**

Frequently created/destroyed objects (enemies, projectiles) cause allocations. An object pool would reuse deactivated GameObjects:

```cpp
class ObjectPool {
    std::vector<std::unique_ptr<GameObject>> m_Pool;
public:
    GameObject* Acquire();  // Reactivate a pooled object
    void Release(GameObject*);  // Deactivate and return to pool
};
```

---

## 10. Known Issues

1. **Unused static counter:** `Scene.cpp` defines `unsigned int m_idCounter = 0;` that appears unused
2. **FixedUPdate typo:** Known typo in FSM interface (capital P) — kept for consistency until refactored
3. **Renderer pointer safety:** Raw pointers in `m_RendererComponents` rely on Component lifetime matching Scene lifetime — safe in practice but could be formalized with a registration/unregistration pattern on Component destruction
