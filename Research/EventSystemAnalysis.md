# Event System — Deep Analysis

## Architecture Overview

The event system is a custom Observer-pattern implementation split across two layers:

- **Observable** — low-level observer list, manages `ObserverEntry` pairs (observer pointer + connection pointer)
- **Event** — high-level wrapper that owns an `Observable` (lazily created) and manages `EventConnection` lifetimes via `shared_ptr`
- **EventInfo** — type-erased event payload carrying an `EventID` (uint32_t hash) and a `unique_ptr<EventArgs>` for the data
- **IObserver** — interface with a single `OnNotify(EventInfo&)` method
- **EventConnection** — lightweight handle for safe disconnection; tracks whether the source `Event` is still alive

Events are defined as structs with a static `ID` (compile-time string hash) and a nested `Args` class deriving from `EventArgs`. A free function `CreateEvent<T>(...)` constructs the `EventInfo` payload in a type-safe way.

```
┌──────────────────────────────────────────────────────────┐
│                    Event (owner)                         │
│  ┌─────────────┐    ┌────────────────────────────────┐   │
│  │ Observable   │    │ vector<shared_ptr<Connection>> │   │
│  │  (lazy init) │    │  (lifetime management)         │   │
│  │  ┌────────┐  │    └────────────────────────────────┘   │
│  │  │Entry[0]│──┼──→ IObserver* + EventConnection*       │
│  │  │Entry[1]│──┼──→ IObserver* + EventConnection*       │
│  │  └────────┘  │                                        │
│  └─────────────┘                                        │
└──────────────────────────────────────────────────────────┘
                        │ Notify(EventInfo&)
                        ▼
              IObserver::OnNotify(EventInfo&)
```

## Event Definition Pattern

Every event follows the same struct convention:

```cpp
struct MyEvent
{
    static const EventID ID{ HashEventID("MyEvent") };  // compile-time hash
    struct Args : public EventArgs
    {
        Args(int _value) : value{ _value } {}
        int value;
    };
};
```

**Creating and firing:**
```cpp
EventInfo e = CreateEvent<MyEvent>(42);
someEvent.Notify(e);
```

**Receiving:**
```cpp
void MyComponent::OnNotify(EventInfo& event) {
    switch (event.GetID()) {
    case MyEvent::ID: {
        auto& args = event.GetArgs<MyEvent>();
        // use args.value
        break;
    }
    }
}
```

## Connection Lifecycle

1. **Subscribe:** `shared_ptr<EventConnection> conn = event.AddObserver(this);` — the Event creates and owns the connection
2. **Receive:** Observer's `OnNotify()` is called whenever `Event::Notify()` fires
3. **Unsubscribe (manual):** `conn->Disconnect(this);` — removes the observer and erases the connection from the Event's list
4. **Unsubscribe (auto on Event death):** `Event::~Event()` sets all connections' `active = false` and clears the Observable — any `shared_ptr<EventConnection>` still held by observers becomes inert (safe, won't dangle)

The observer holds a `shared_ptr<EventConnection>` as proof of subscription. The Event also holds a `shared_ptr` to the same connection (shared ownership). This design means:
- If the **Event** dies first → connections are marked inactive, observers can safely check `active` before disconnecting
- If the **Observer** dies first → observer must call `Disconnect()` in its destructor, otherwise the Event holds a dangling `IObserver*`

## All Events in the Codebase

### Engine Events (JREngine)

| Event | ID String | Args | Fired By | Purpose |
|-------|-----------|------|----------|---------|
| `Events::EventDestroyed` | `"EventDestroyed"` | `const Event&` | (defined but not observed anywhere) | Intended to notify when an Event object is destroyed |
| `Events::Box2DCollisionEvent` | `"Box2DCollision"` | `Box2DColliderComponent&, ICollider&` | `Box2DColliderComponent::OnCollisionWith()` | Physics collision notification |
| `Events::EndOfClipEvent` | `"EndOfClip"` | `SpriteAnimationClip*` | `SpriteAnimationClip::Update()` | Animation clip reached end (loop or pong bounce) |

### Game Events (BubbleBobble)

| Event | ID String | Args | Fired By | Purpose |
|-------|-----------|------|----------|---------|
| `Events::HealthChanged` | `"HealthChanged"` | `int amount, int newHealth` | `HealthComponent::TakeDamage()` | Health value changed |
| `Events::PlayerLostLive` | `"PlayerLostLive"` | `int health` | `PlayerState_Dead::OnEvent()` | Player death animation completed, life lost |
| `Events::EnemyDied` | `"EnemyDied"` | `int points` | `ZenchanScriptComponent::Kill()` | Enemy was killed, carries point value |
| `Event::IncreasedScore` | `"IncreasedScore"` | `int newScore` | `ScoreComponent::IncreaseScore()` | Score changed |

## Event Flow Diagrams

### Collision → Player Death → Game Over

```
BoxPhysicsSystem detects overlap
  → Box2DColliderComponent::OnCollisionWith()
    → OnCollisionEvent.Notify(Box2DCollisionEvent)
      → PlayerScriptComponent::OnNotify()
        → PlayerState_Mortal::OnEvent()
          → checks collision layer is Enemy
          → RequestStateChange(Dead)

[next Update: state transitions to Dead]

  → PlayerState_Dead::OnEnter()
    → plays Death animation

[Death animation reaches end]

  SpriteAnimationClip::Update()
    → OnEndOfClipEvent.Notify(EndOfClipEvent)
      → PlayerScriptComponent::OnNotify()
        → PlayerState_Dead::OnEvent()
          → HealthComponent::TakeDamage(1)
            → OnHealthChanged.Notify(HealthChanged)
              → HealthUIComponent::OnNotify() [updates hearts display]
          → OnPlayerLostLive.Notify(PlayerLostLive)
            → InGameState::OnNotify()
              → if health <= 0: m_PlayerDied = true → GameOverScreen
              → else: respawn at spawn position
```

### Bubble Traps Enemy → Player Pops → Score

```
BoxPhysicsSystem detects Bubble ↔ Enemy overlap
  → BubbleScript::OnNotify(Box2DCollisionEvent)
    → enemy layer? → ZenchanScriptComponent::SetTrappedBy(bubble)
      → enemy follows bubble position

BoxPhysicsSystem detects Bubble ↔ Player overlap
  → BubbleScript::OnNotify(Box2DCollisionEvent)
    → friendly layer + alive > 0.5s? → PopAndKill()
      → ZenchanScriptComponent::Kill()
        → OnEnemyDied.Notify(EnemyDied{100})
          → InGameState::OnNotify()
            → increment score, update score text
            → if all enemies killed → load next level
      → Burst() → play Poof animation

[Poof animation ends]
  → BubbleScript::OnNotify(EndOfClipEvent)
    → GetGameObject().Destroy()
```

## What's Good

1. **Clean event definition pattern** — the struct-with-nested-Args convention is consistent everywhere and easy to follow. `CreateEvent<T>(...)` provides type safety with `std::constructible_from` constraint.

2. **Compile-time hashed IDs** — `HashEventID()` is `constexpr`, so event IDs are resolved at compile time. This avoids string comparisons at runtime while keeping events human-readable in code.

3. **Safe connection lifecycle** — the `shared_ptr<EventConnection>` design handles the "Event dies before Observer" case gracefully. The `active` flag prevents use-after-free on stale connections.

4. **Lazy Observable allocation** — `Event` only creates its `Observable` on first `AddObserver()`. Events that are never subscribed to cost zero heap allocations.

5. **Decoupled communication** — components like `InGameState` can observe `PlayerLostLive` and `EnemyDied` without any direct coupling to the player or enemy implementations. The game manager doesn't need to know about collision details.

6. **State machine integration** — `IObserver::OnNotify()` feeds into `PlayerStateBase::OnEvent()`, meaning collision and animation events are routed through the state machine automatically. Each state only handles the events it cares about.

## Issues Found

### 1. Observer Destruction is Entirely Manual (Dangling Pointer Risk)

When an `IObserver` is destroyed, it **must** manually call `Disconnect()` for every connection it holds. There is no RAII guard — forgetting a disconnect leaves a dangling `IObserver*` inside the `Observable`, which will crash on the next `Notify()`.

**Example of the pattern every observer must follow:**
```cpp
BubbleScript::~BubbleScript() {
    if (m_Box2DCollisionEventConn)
        m_Box2DCollisionEventConn->Disconnect(this);
    if (m_PoofAnimClipEndOfClipEventConn)
        m_PoofAnimClipEndOfClipEventConn->Disconnect(this);
}
```

**Fix:** A RAII wrapper (e.g., `ScopedEventConnection`) that disconnects in its destructor would eliminate this entire class of bugs.

### 2. InGameState Doesn't Disconnect from EnemyDied Events

In `InGameState::CreateEnemies()`:
```cpp
zenchanScriptCmp->OnEnemyDied.AddObserver(this);
```
The returned `shared_ptr<EventConnection>` is **discarded** — it's not stored anywhere. This means `InGameState` has no way to disconnect from individual enemy events. It relies on enemies being destroyed (which invalidates their Event objects) before the `InGameState` is destroyed.

This works **by accident** because scene cleanup destroys enemies before game states. But it's fragile — if destruction order ever changes, `InGameState`'s `OnNotify()` could be called on a partially-destroyed object, or the enemy's Event could try to notify a destroyed `InGameState`.

**Fix:** Store the connections (e.g., in a `vector<shared_ptr<EventConnection>>`) and disconnect them in `OnExit()` or the destructor.

### 3. Box2DCollisionEvent Connection Not Stored by PlayerScriptComponent

In the `PlayerScriptComponent` constructor:
```cpp
m_pBox2DColliderCmp->OnCollisionEvent.AddObserver(this);
```
The returned connection is discarded. The PlayerScriptComponent stores connections for `EndOfClipEvent` but not for `OnCollisionEvent`. Same problem as issue #2 — no explicit disconnect possible, relying on destruction order.

### 4. No Event Filtering — All Observers Get All Events

`Observable::NotifyObservers()` broadcasts to every observer regardless of which `EventID` they care about. Every observer must switch on `event.GetID()` and ignore irrelevant events. For a small game this is fine, but it means:
- Observers that only care about `EndOfClipEvent` still receive `Box2DCollisionEvent` (if subscribed to a source that fires both — though currently each `Event` member only fires one type)
- No way to subscribe to a specific event ID on a given Event object

In practice this is mitigated because each `Event` member (e.g., `OnCollisionEvent`, `OnEndOfClipEvent`) only fires one type of event. But the system doesn't enforce this — nothing prevents `OnCollisionEvent.Notify()` from being called with an `EndOfClipEvent` payload.

### 5. EventDestroyed Is Defined but Never Used

`Events::EventDestroyed` is declared in `Event.h` but `Event::~Event()` never fires it. The destructor just marks connections as inactive and clears the observable. If the intent was to notify observers that an Event object was dying, the implementation is missing.

### 6. HashEventID Has No Collision Detection

`HashEventID()` uses a simple polynomial hash (SDBM variant). Two different strings could produce the same `uint32_t`, causing events to be silently misrouted. There's no compile-time or runtime check for collisions.

For the current ~7 events this is extremely unlikely, but as the event count grows, this becomes a real risk. A `static_assert` or debug-mode registry that checks for duplicates would catch this.

### 7. FalsePredicate Returns True

In `State.h`:
```cpp
class FalsePredicate : public IPredicate {
    virtual bool Evaluate() const override { return true; };  // BUG: should be false
};
```
Not directly event-related but in the same header. `FalsePredicate::Evaluate()` returns `true` instead of `false`.

### 8. EventInfo Move Semantics Are Redundant

`EventInfo` has explicit move constructor/assignment that just do what the compiler-generated versions would do (move the ID and unique_ptr). Since `EventID` is a trivial `uint32_t` and `unique_ptr` is already movable, the Rule of Five implementations add maintenance cost for no benefit. Could use `= default`.

### 9. Mutable Everything in Event and Observable

Both `Event` and `Observable` mark their data members as `mutable`, allowing `AddObserver()`, `RemoveObserver()`, and the collections to be modified through const references. This is done so `const` Events can still be subscribed to, but it defeats the purpose of const correctness entirely. Any function receiving a `const Event&` can mutate its observer list.

### 10. NotifyObservers Iterates While Observers Can Be Modified

`Observable::NotifyObservers()` iterates `m_Observers` with a range-for loop. If an observer's `OnNotify()` calls `RemoveObserver()` (e.g., to unsubscribe itself), the vector is modified during iteration — this is **undefined behavior**.

Currently no observer unsubscribes during notification, but this is a latent crash waiting to happen.

**Fix:** Copy the observer list before iterating, or use a deferred-removal pattern (mark entries for removal, clean up after the loop).

## Event Ownership Map

```
JREngine:
  Box2DColliderComponent ── owns ── OnCollisionEvent (fires Box2DCollisionEvent)
  SpriteAnimationClip    ── owns ── OnEndOfClipEvent (fires EndOfClipEvent)

BubbleBobble:
  HealthComponent        ── owns ── OnHealthChanged  (fires HealthChanged)
  PlayerScriptComponent  ── owns ── OnPlayerLostLive (fires PlayerLostLive)
  ZenchanScriptComponent ── owns ── OnEnemyDied      (fires EnemyDied)
  ScoreComponent         ── owns ── OnIncreasedScoreEvent (fires IncreasedScore)
```

## Observer Subscription Map

| Observer | Subscribes To | Event Type | Connection Stored? |
|----------|--------------|------------|-------------------|
| `PlayerScriptComponent` | `Box2DColliderComponent::OnCollisionEvent` | Box2DCollisionEvent | **No** (discarded) |
| `PlayerScriptComponent` | `SpriteAnimationClip::OnEndOfClipEvent` (Shoot) | EndOfClipEvent | Yes (`m_ShootClipEndOfClipEventConn`) |
| `PlayerScriptComponent` | `SpriteAnimationClip::OnEndOfClipEvent` (Death) | EndOfClipEvent | Yes (`m_DeathClipEndOfClipEventConn`) |
| `BubbleScript` | `Box2DColliderComponent::OnCollisionEvent` | Box2DCollisionEvent | Yes (`m_Box2DCollisionEventConn`) |
| `BubbleScript` | `SpriteAnimationClip::OnEndOfClipEvent` (Poof) | EndOfClipEvent | Yes (`m_PoofAnimClipEndOfClipEventConn`) |
| `HealthUIComponent` | `HealthComponent::OnHealthChanged` | HealthChanged | Yes (`m_HealthCmpOnHealthChangedEventConn`) |
| `InGameState` | `PlayerScriptComponent::OnPlayerLostLive` | PlayerLostLive | Yes (`m_PlayerLostLifeEventConn`) |
| `InGameState` | `ZenchanScriptComponent::OnEnemyDied` | EnemyDied | **No** (discarded) |
| `PortalScriptComponent` | `Box2DColliderComponent::OnCollisionEvent` | Box2DCollisionEvent | Yes (`m_Box2DCollisionEventConn`) |

## Verdict

The event system is well-designed for a school/exam engine. The struct-based event definition pattern with compile-time hashing is clean and extensible. The `EventConnection` mechanism for safe disconnection shows thoughtful lifetime management.

The main risks are: (1) manual disconnect burden with no RAII safety net — every observer destructor must remember to disconnect, (2) two places where connections are discarded making explicit disconnect impossible, and (3) the iterator-invalidation hazard in `NotifyObservers`. For BubbleBobble's scope these work fine, but they'd be the first things to address for a production engine.

---

## Suggestions for Improvement

### Priority 1: Self-Disconnecting EventConnection
The single highest-impact improvement. Change `EventConnection` to store both sides of the subscription and auto-disconnect in its destructor:

```cpp
struct EventConnection {
    const Event* event = nullptr;   // nullptr means disconnected/inactive
    IObserver* observer = nullptr;

    ~EventConnection() {
        if (event) event->RemoveObserver(observer);
    }
    // Move-only, no copy
};
```

The ownership model changes so that **the Observer owns the EventConnection via `shared_ptr`, and the Event holds only a `weak_ptr` to it**:

- `Event::m_Connections` becomes `std::vector<std::weak_ptr<EventConnection>>`
- `Event::AddObserver()` returns `std::shared_ptr<EventConnection>` (observer stores this as a member)
- `Event::Notify()` locks each `weak_ptr` and skips expired ones (lazy cleanup)

**Safe cleanup from both sides — no manual disconnect needed:**

**Observer dies first** → its `shared_ptr<EventConnection>` member drops → ref count hits zero → `EventConnection` destructor fires → `event->RemoveObserver(observer)` removes the now-dangling `IObserver*` from the Observable before it can be called → Event's `weak_ptr` expires and is skipped/cleaned on next `Notify`.

**Event dies first** → `Event::~Event()` locks each `weak_ptr`, nulls out `conn->event` on all live connections, then clears the Observable → any `shared_ptr<EventConnection>` still held by observers is now inert (`event == nullptr`) → when those observers are eventually destroyed their `EventConnection` destructor is a no-op.

All existing `shared_ptr<EventConnection>` members on observers remain unchanged — destruction is now fully automatic by virtue of them being members.

### Priority 2: Fix Discarded Connections
Two places currently discard the return value of `AddObserver()`:
- `PlayerScriptComponent` discards its `OnCollisionEvent` connection
- `InGameState` discards all `OnEnemyDied` connections

Store these connections (a `std::vector<std::shared_ptr<EventConnection>>` for InGameState's multiple enemies). With Priority 1 implemented, this becomes trivial — just store the `shared_ptr<EventConnection>` and lifetime is automatic.

### Priority 3: Safe Iteration in NotifyObservers
`Observable::NotifyObservers()` iterates the observer vector directly. If any observer unsubscribes during notification, the vector is modified during iteration (undefined behavior). Two approaches:
- **Copy-on-iterate**: Copy the observer list before the loop. Simple, small overhead for the current observer counts.
- **Deferred removal**: Mark entries as "pending removal" during iteration, clean up after the loop. More efficient for large observer lists.

The copy approach is recommended for simplicity — observer lists are small (1-3 entries typically).

### Priority 4: Remove EventDestroyed or Implement It
`Events::EventDestroyed` is defined but never fired. Either:
- Remove it entirely (it's dead code)
- Wire it up in `Event::~Event()` if there's a use case for observing event lifetime

### Priority 5: Fix FalsePredicate
`FalsePredicate::Evaluate()` returns `true` instead of `false`. One-line fix, but in a different file (`State.h`). Fix it to avoid confusion if state machine predicates are extended.

### Priority 6: Simplify EventInfo (Rule of Zero)
Remove the explicit move constructor/assignment from `EventInfo`. The compiler-generated versions do the same thing (`uint32_t` is trivially copyable, `unique_ptr` is already movable). Use `= default` or just remove them entirely.

### Priority 7: Hash Collision Detection (Debug Mode)
Add a debug-mode registry that checks for `HashEventID` collisions:
```cpp
#ifdef _DEBUG
inline std::unordered_map<EventID, std::string>& GetEventRegistry() {
    static std::unordered_map<EventID, std::string> registry;
    return registry;
}
inline EventID RegisterAndCheckEventID(const char* name) {
    EventID id = HashEventID(name);
    auto [it, inserted] = GetEventRegistry().emplace(id, name);
    assert(inserted || it->second == name); // Collision detected!
    return id;
}
#endif
```
With only ~7 events this is extremely unlikely to matter, but it's a safety net as the event count grows.

### Priority 8: Reconsider Mutable Members
`Event` and `Observable` mark all data as `mutable` so const Events can be subscribed to. This defeats const correctness — any function with a `const Event&` can silently mutate the observer list. Consider whether `AddObserver()` really needs to work on const Events, or if the const-ness can be relaxed at the call sites instead.
