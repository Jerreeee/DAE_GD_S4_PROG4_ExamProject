# BoxPhysicsSystem — Complete Analysis

## Architecture Overview

The physics system follows a **Service Locator** pattern: `IPhysicsSystem` interface accessed globally via `ServiceLocator`, with `BoxPhysicsSystem` as the concrete implementation. It handles two distinct collision domains: **static colliders** (tilemaps/platforms — registered in groups) and **dynamic colliders** (player/enemies/bubbles — registered individually). Collision filtering uses a **layer/mask bitflag** system.

## Class Hierarchy

```
IPhysicsSystem (IPhysicsSystem.h)
├── NullPhysicsSystem (IPhysicsSystem.h:25-39) — no-op fallback
└── BoxPhysicsSystem (BoxPhysicsSystem.h/.cpp) — concrete implementation

ICollisionShape (ICollider.h:15-19)
└── BoxShape (ICollider.h:21-116) — AABB with offset, width, height

ICollider (ICollider.h:135-154)
└── Box2DColliderComponent (Box2DColliderComponent.h/.cpp) — component on GameObjects

RigidBody2DComponent (RigidBody2DComponent.h) — standalone kinematic body, NOT integrated with BoxPhysicsSystem
```

## Core Data Structures

### BoxShape (ICollider.h:21-116)
```cpp
struct BoxShape : ICollisionShape {
    glm::vec2 offset{};     // Offset from owning GameObject position
    float width{};
    float height{};
    static constexpr float epsilon = 0.0001f;

    Region GetRegion(glm::vec2 base) const;        // World-space AABB
    BoxShape Translated(glm::vec2 delta) const;     // New shape shifted by delta
    bool Intersects(other, baseA, baseB) const;     // AABB overlap test
    bool OverlapInX/Y(...) const;                   // Per-axis overlap
    bool FullOverlapInX/Y(...) const;               // Full containment
    float GetXDist/GetYDist(...) const;             // Signed distance
};
```

### ColliderProperties (ICollider.h:118-125)
```cpp
struct ColliderProperties {
    uint32_t layer = 0;      // What this object IS (bitflag)
    uint32_t mask = 0;       // What this object COLLIDES WITH (bitflag)
    bool isStatic = false;   // Static colliders go in the static list
    bool isTrigger = false;  // Currently unused by BoxPhysicsSystem
};
```

### StaticCollider (ICollider.h:127-132)
```cpp
struct StaticCollider {
    std::unique_ptr<ICollisionShape> shape;
    ColliderProperties properties;
    uint32_t group;           // For batch unregistration (e.g. per tilemap)
};
```

### CollisionDir (BoxPhysicsSystem.h:10-25)
```cpp
struct CollisionDir {
    bool left, right, up, down;
    bool Any() const;
    bool X() const;       // left || right
    bool Y() const;       // up || down
    bool Corner() const;  // X && Y simultaneously
};
```

### CollisionDirInfo (BoxPhysicsSystem.h:27-32)
```cpp
struct CollisionDirInfo {
    float collPos;                                              // Where collision happened
    float entryDist{ std::numeric_limits<float>::max() };       // Distance to collision
    float lambda{ std::numeric_limits<float>::max() };          // Time-of-entry (0–1)
};
```

### CollisionInfo (BoxPhysicsSystem.h:49-60) — Output
```cpp
struct CollisionInfo {
    CollisionDirInfo left, right, up, down;
    CollisionDir collDir;      // Which sides collided
    glm::vec2 newPos;          // Corrected world position
    glm::vec2 velOut;          // Velocity after collision (zeroed on collision axis)
};
```

### CollisionSettings (BoxPhysicsSystem.h:74-84) — Input
```cpp
struct CollisionSettings {
    glm::vec2 oldPos;
    const ICollider& collider;
    float dt;
    glm::vec2 vel;
    bool applyGravity{ true };

    using CollisionFilterFunc = std::function<CollisionDir(const StaticCollider&, const CollisionSettings&)>;
    CollisionFilterFunc filterFunc;   // Per-collider custom filtering callback
};
```

## Collision Layer System (BubbleBobble/src/EngineSetup.h)

### Layers (what the object IS)
```cpp
static constexpr uint32_t Friendly    = 1 << 0;   // Player
static constexpr uint32_t Enemy       = 1 << 1;   // Enemies
static constexpr uint32_t StaticLevel = 1 << 2;   // Walls/boundaries
static constexpr uint32_t Platform    = 1 << 3;   // One-way platforms
static constexpr uint32_t Bubble      = 1 << 4;   // Bubbles
```

### Masks (what the object COLLIDES WITH)
```cpp
static constexpr uint32_t Friendly          = Enemy | StaticLevel | Platform | Bubble;
static constexpr uint32_t Enemy             = Friendly | StaticLevel | Platform | Bubble;
static constexpr uint32_t DynamicGameObject = Friendly | Enemy;   // Portal use
static constexpr uint32_t StaticLevel       = Friendly | Enemy;
static constexpr uint32_t Platform          = Friendly | Enemy;
static constexpr uint32_t Bubble            = Friendly | Enemy;
```

**Check** (BoxPhysicsSystem.cpp:23): Both directions must match:
```cpp
if (!((propsA.layer & propsB.mask) && (propsB.layer & propsA.mask)))
    continue;
```

## BoxPhysicsSystem Members (BoxPhysicsSystem.h:95-100)

```cpp
std::unordered_set<uint32_t> m_StaticColliderGroups{};  // Allocated group IDs
std::vector<StaticCollider> m_StaticColliders{};         // ALL static colliders (flat list)
std::vector<ICollider*> m_DynamicColliders{};            // ALL dynamic colliders (raw ptrs)
float m_Gravity{ 9.81f };
float m_WorldScale{ 1.f };
float m_VelScale{ 1.f };     // Computed as 1 / m_WorldScale
```

## Key Methods

### Update() — Dynamic-vs-Dynamic (BoxPhysicsSystem.cpp:7-42)
- Nested loop: every dynamic collider against every other dynamic collider
- **O(n^2)** complexity, no spatial partitioning
- Layer/mask filtering skips non-interacting pairs
- Calls `OnCollisionWith()` on both colliders (event notification only — no position correction)
- No swept collision — just instantaneous AABB overlap test

### MoveCollider() — Dynamic-vs-Static Sweep (BoxPhysicsSystem.cpp:80-255)
- **Called manually by game code** (not by the engine loop)
- Swept AABB collision: separates X and Y axes
- Calculates **lambda** (time of entry) to determine which axis collision happens first
- Applies gravity if `applyGravity == true`
- Supports custom `filterFunc` callback for per-collider behavior (e.g. one-way platforms)
- Iterates over ALL static colliders — **O(m)** per dynamic object
- Returns corrected position + velocity in `CollisionInfo`

### Static Collider Management
- `GetFreeStaticGroup()` — linear scan of `m_StaticColliderGroups` for next available ID
- `RegisterStaticCollider()` — push_back to flat vector
- `UnegisterStaticCollidersByGroup()` — `std::erase_if` by group ID (note: typo in name)

### Dynamic Collider Management
- `RegisterCollider()` — push_back raw pointer, rejects static colliders
- `UnregisterCollider()` — `std::erase` from vector

## Update Flow

```
JREngine::RunOneFrame()
  │
  while (lag >= fixedTimeStep):
  ├── SceneManager::FixedUpdate()
  │     └── PlayerScriptComponent::FixedUpdate()
  │           └── MoveCollider()                          ← manual call
  │                 └── BoxPhysicsSystem::MoveCollider()   ← dynamic vs ALL static
  │
  └── ServiceLocator::GetPhysicsSystem().Update()          ← engine call
        └── BoxPhysicsSystem::Update()                     ← dynamic vs dynamic (events only)
```

**Critical observation**: `MoveCollider()` is NOT called by the engine. Each game script that needs physics must manually call it. The engine only calls `Update()` which handles dynamic-vs-dynamic overlap detection.

## How Game Code Uses It

### Player (PlayerScriptComponent.cpp:136-154)
```cpp
void PlayerScriptComponent::MoveCollider() {
    BoxPhysicsSystem::CollisionSettings cs{ oldPos, *m_pBox2DColliderCmp };
    cs.dt = Timer::GetInstance().GetFixedTimeStep();
    cs.applyGravity = true;
    cs.filterFunc = Utils::PlatformCollisionDirFilterFunc;
    cs.vel = m_Vel;

    // Must downcast from IPhysicsSystem to BoxPhysicsSystem
    BoxPhysicsSystem& physicsSystem = static_cast<BoxPhysicsSystem&>(ServiceLocator::GetPhysicsSystem());
    physicsSystem.MoveCollider(cs, m_CollInfo);
    GetGameObject().SetWorldPosition(m_CollInfo.newPos.x, m_CollInfo.newPos.y);
    m_Vel = m_CollInfo.velOut;
}
```

### One-Way Platform Filter (Utils.cpp:37-53)
```cpp
CollisionDir PlatformCollisionDirFilterFunc(const StaticCollider& collider,
                                            const CollisionSettings& cs) {
    // Platforms only collide from above (down direction)
    // Skip if already overlapping Y (so you can jump through from below)
    bool isPlatform = collider.properties.layer & CollisionLayer::Platform;
    bool alreadyCollidingY = worldBox.OverlapInY(colliderBox);
    bool skipDownColl = isPlatform && alreadyCollidingY;

    collDir.up = !isPlatform;
    collDir.down = !skipDownColl;
    collDir.left = !isPlatform;
    collDir.right = !isPlatform;
    return collDir;
}
```

### TileMap Registration (TileMapComponent.cpp:16-32)
```cpp
void TileMapComponent::OnEnable() {
    uint32_t group = physics.GetFreeStaticGroup();
    for (auto& ci : colliderInfos) {
        StaticCollider sc;
        sc.shape = std::make_unique<BoxShape>(ci.boxShape);
        sc.group = group;
        sc.properties.layer = ci.isPlatform ? CollisionLayer::Platform : CollisionLayer::StaticLevel;
        sc.properties.mask  = ci.isPlatform ? CollisionMask::Platform  : CollisionMask::StaticLevel;
        physics.RegisterStaticCollider(std::move(sc));
    }
}
```

### Dynamic Collision Events (Box2DColliderComponent.cpp)
- `OnEnable()` → registers with physics system
- `OnDisable()` → unregisters
- `OnCollisionWith()` → fires `Box2DCollisionEvent` to observers

## Dual System: Static vs Dynamic — The Core Design Split

The system fundamentally treats static and dynamic colliders as **two completely separate worlds**:

| Aspect | Static Colliders | Dynamic Colliders |
|--------|-----------------|-------------------|
| Storage | `std::vector<StaticCollider>` (owned) | `std::vector<ICollider*>` (raw ptrs) |
| Registration | `RegisterStaticCollider()` with group ID | `RegisterCollider()` via component lifecycle |
| Collision check | In `MoveCollider()` — swept AABB | In `Update()` — instantaneous overlap |
| Who calls it | Game scripts manually | Engine automatically |
| Response | Position correction + velocity zeroing | Event notification only |
| Layer/mask | Set at registration time, never changes | Read from component each frame |

**Why this matters**: A dynamic object that needs to collide with static geometry must manually call `MoveCollider()`. The engine's `Update()` only handles dynamic-vs-dynamic. This creates a split where:
1. Static collision resolution is **opt-in per script**
2. Dynamic collision is **automatic but notification-only**
3. There is **no dynamic-vs-static collision that's automatic** — the gap in the middle

## RigidBody2DComponent — The Orphan

`RigidBody2DComponent` exists in the engine but is **not integrated** with `BoxPhysicsSystem`:
- Has its own gravity constant (40.f vs BoxPhysicsSystem's 9.81f)
- Applies velocity directly to transform in its own `FixedUpdate()`
- No collision detection or resolution
- Game code prefers manual `MoveCollider()` calls instead

---

## Issues & Scalability Concerns

### 1. No Spatial Partitioning — O(n^2) and O(n*m)
- `Update()`: Every dynamic collider checked against every other — O(n^2)
- `MoveCollider()`: Each moving object sweeps against ALL static colliders — O(m) per call
- With 20 dynamic + 500 static colliders = 10,000 static checks + 190 dynamic checks per frame
- **Current scope**: ~10-20 dynamic, ~30 static per level — fine for now

### 2. MoveCollider() Not in IPhysicsSystem Interface
- Game code must `static_cast<BoxPhysicsSystem&>(ServiceLocator::GetPhysicsSystem())` to call `MoveCollider()`
- Defeats the purpose of the interface abstraction
- If physics implementation ever changes, all game scripts break

### 3. Manual MoveCollider() Calls — No Automatic Physics
- Every script that needs static collision must manually call `MoveCollider()` in its `FixedUpdate()`
- Easy to forget, duplicates boilerplate, no consistency guarantee
- Enemies, player, bubbles all have their own movement+collision code

### 4. RigidBody2DComponent is Disconnected
- Exists but doesn't integrate with BoxPhysicsSystem
- Has different gravity value
- No collision resolution — just moves the transform
- Creates confusion about which system to use

### 5. No Dynamic-vs-Dynamic Position Correction
- `Update()` fires events but doesn't resolve overlaps
- If two dynamic objects overlap, they stay overlapping
- Only game logic can respond (e.g. destroy bubble on contact)
- Fine for triggers, but prevents physics-based dynamic interactions

### 6. isTrigger Flag is Unused
- `ColliderProperties::isTrigger` exists but is never checked
- No trigger vs collider distinction in the physics system

### 7. Static Colliders Have No Spatial Organization
- Flat vector with no grouping by position
- Every `MoveCollider()` call iterates the entire list
- Group IDs only serve batch unregistration — not spatial queries

### 8. Typo in Interface
- `UnegisterStaticCollidersByGroup()` — missing 'r' in "Unregister"
- Propagated through interface and implementation

---

## Suggestions for Improvement

### Priority 1: Add MoveCollider to the Interface
Move `MoveCollider()`, `CollisionSettings`, and `CollisionInfo` to `IPhysicsSystem` so game code doesn't need to downcast. This is the lowest-effort, highest-impact change:
```cpp
// In IPhysicsSystem:
virtual void MoveCollider(const CollisionSettings& cs, CollisionInfo& ci) = 0;
```

### Priority 2: Automatic Physics Step for Dynamic Colliders
Instead of requiring every script to manually call `MoveCollider()`, have `BoxPhysicsSystem::Update()` automatically resolve dynamic-vs-static collisions for all registered dynamic colliders that have a velocity. This would:
- Eliminate boilerplate in every script
- Ensure consistent physics behavior
- Allow `RigidBody2DComponent` to actually work with the physics system

A `RigidBody2DComponent` could store velocity and let the physics system move it:
```cpp
void BoxPhysicsSystem::Update() {
    // 1. For each dynamic collider with a rigidbody:
    //    - Build CollisionSettings from rigidbody velocity
    //    - Call MoveCollider() internally
    //    - Write back position and velocity
    // 2. Then do dynamic-vs-dynamic overlap checks (existing logic)
}
```

### Priority 3: Spatial Partitioning (Grid)
For a tile-based game like Bubble Bobble, a **uniform grid** is the simplest and most effective spatial structure:
- Divide the level into cells (e.g. 32x32 pixels)
- Each static collider is placed in the cells it overlaps
- `MoveCollider()` only checks colliders in the cells the moving object passes through
- Reduces static checks from O(m) to O(k) where k is typically 4-8 colliders

A grid is preferable to a quadtree here because:
- Tile-based levels have uniform collider distribution
- Simpler to implement and debug
- O(1) cell lookup vs O(log n) tree traversal
- Static colliders rarely change, so rebuild cost doesn't matter

### Priority 4: Implement Trigger Support
The `isTrigger` flag already exists — wire it up:
- Triggers detect overlap but don't correct position
- Useful for kill zones, pickups, level transitions
- Currently these are handled ad-hoc through dynamic-vs-dynamic events

### Priority 5: Broad Phase for Dynamic-vs-Dynamic
Add a quick AABB distance check or sort-and-sweep before the O(n^2) loop:
```cpp
// Sort by X position, only check nearby pairs
std::sort(m_DynamicColliders.begin(), m_DynamicColliders.end(), byXPosition);
for (size_t i = 0; i < n; ++i) {
    for (size_t j = i + 1; j < n && withinRange(i, j); ++j) {
        // Full collision check
    }
}
```

### Priority 6: Unify Gravity Constants
`BoxPhysicsSystem` uses 9.81f, `RigidBody2DComponent` uses 40.f. Pick one source of truth — the physics system's `m_Gravity` — and have everything read from there.

### Priority 7: Fix the Typo
Rename `UnegisterStaticCollidersByGroup` → `UnregisterStaticCollidersByGroup` across the codebase. Small but it's a public API.
