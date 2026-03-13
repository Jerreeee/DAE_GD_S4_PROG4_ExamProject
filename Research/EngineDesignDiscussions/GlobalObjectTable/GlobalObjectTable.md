# Global Object Table — Design Discussion

This document captures the full design discussion around implementing a Global Object Table (generational handle system) for JREngine, similar to Unreal's TWeakObjectPtr.

---

## 1. The Problem: Dangling Pointers

### Event System (Starting Point)

The event system analysis identified "Observer Destruction is Entirely Manual (Dangling Pointer Risk)" — when an `IObserver` is destroyed, it must manually call `Disconnect()` for every connection. Forgetting leaves a dangling `IObserver*` inside the `Observable`.

### Can We Auto-Disconnect in EventConnection's Destructor?

**Question:** Is it safe to move Disconnect to `~EventConnection()`?

**Answer: No.** `EventConnection` is shared between two owners via `shared_ptr`:

```
Event::m_Connections  --> shared_ptr<EventConnection> <--  Observer (member)
                            refcount = 2
```

`~EventConnection()` runs when the **last** `shared_ptr` dies (refcount hits 0), not when the observer specifically dies:

- **Observer dies first:** refcount drops to 1 (Event still holds its shared_ptr) -> destructor does NOT run. The dangling `IObserver*` is exactly the bug we wanted to fix.
- **Event dies first:** Event sets `active = false`, drops its shared_ptr -> refcount drops to 1. Later observer dies -> destructor runs, but Event is already dead -> use-after-free.
- **Both dead:** destructor runs but both are gone -> nothing safe to call.

### The Fix: Change Ownership Model

Make the Event hold `weak_ptr` instead of `shared_ptr`. Observer becomes sole owner:

```
Event  --weak_ptr----> EventConnection <--shared_ptr-- Observer
                        refcount = 1
```

Now `~EventConnection()` fires when the observer dies — exactly when needed.

**Do we still need the `active` flag?** Yes. If the Event dies first, `event` pointer in EventConnection is dangling. `Event::~Event()` must lock each `weak_ptr` and set `event = nullptr` (or `active = false`) so the destructor knows not to touch the dead Event.

**Alternative:** Just null out the `event` pointer instead of a separate bool. The pointer itself doubles as the liveness check.

---

## 2. How shared_ptr / weak_ptr Works

`shared_ptr` allocates a **control block** alongside the object:

```
std::make_shared<Foo>()

  +---------------------------+
  |      Control Block        |
  |  strong_count = 1         |  <- shared_ptrs
  |  weak_count   = 0         |  <- weak_ptrs
  |                           |
  |  Foo object               |
  +---------------------------+
       ^             ^
   shared_ptr     weak_ptr
```

Both point to the **control block**, not the object directly.

- **Object destruction:** strong_count hits 0 -> `~Foo()` runs, but control block stays alive (weak_ptrs need it)
- **Control block freed:** both counts hit 0

`weak_ptr::expired()` is essentially: `return control_block->strong_count == 0;`

`weak_ptr::lock()` atomically checks strong_count and returns a temporary `shared_ptr` if alive.

### Why Not unique_ptr + weak_ptr?

`weak_ptr` only works with `shared_ptr` — it needs the control block. `unique_ptr` has no control block. Even if the observer is the sole owner, we still need `shared_ptr` for the control block that makes `weak_ptr` possible.

Common pattern: `shared_ptr` + `weak_ptr` doesn't mean "multiple owners." It often means "one owner + others that need to safely check liveness."

### Intrusive Reference Counting?

Can't combine with `std::weak_ptr`. With intrusive counting, the counter lives inside the object — when the object dies, the counter is gone. You'd have to build your own weak reference (shared flag), which just reinvents `shared_ptr`/`weak_ptr` for a `bool`.

---

## 3. How Other Engines Handle Events

### Centralized Event Queue (Unreal, Unity)
One global queue. Events are data, not function calls. No direct pointer between sender and receiver. Tradeoff: 1+ frame latency.

### Managed Delegates (Unreal)
`FMulticastDelegate` stores `TWeakObjectPtr<AMyActor>`. Checks liveness before every dispatch — dead objects silently skipped. Requires a managed object system.

### Signal/Slot (Boost.Signals2, Qt)
Uses `shared_ptr`/`weak_ptr` internally. `scoped_connection` provides RAII disconnect. This is what our `shared_ptr`/`weak_ptr` fix would replicate.

### Manual (EnTT, current JREngine)
Programmer's responsibility. Zero overhead, easy to mess up.

---

## 4. Could JREngine Use Unreal's Approach?

**Question:** Our GameObjects are managed by the engine — can we just check liveness before dispatching, like Unreal?

**Answer: Not directly.** "Managed by the engine" != "safely checkable after destruction." You can't check a flag on a dead object — the memory is freed:

```cpp
// Component destroyed, unique_ptr freed, memory gone
// IObserver* is dangling
if (!observer->GetGameObject().IsDestroyed())  // use-after-free!
    observer->OnNotify(event);
```

Unreal doesn't check a flag on the object. It uses a **global indirection table**:

```
TWeakObjectPtr stores {index, generation} into GUObjectArray

  GUObjectArray:
  [0]  AMyActor*   gen=3
  [1]  nullptr      gen=5       <- destroyed, generation bumped
  [2]  ABubble*    gen=1

  TWeakObjectPtr { index=1, generation=4 }
    -> looks up [1] -> gen 5 != 4 -> dead -> never touches object memory
```

---

## 5. Why Unreal Built a Global Table

Not just for events — they need weak references to any UObject from anywhere:
- AI blackboard targets
- Timers referencing actors
- Animation notifies
- Gameplay abilities targeting actors
- Blueprint variables
- Delegates/events (just one use case)

Benefits over shared_ptr/weak_ptr everywhere:
- One system for all weak references
- Raw pointer performance for strong references
- `TWeakObjectPtr` = two integers, no atomic refcounting
- Works with their garbage collector

---

## 6. Dangling Pointer Audit in JREngine

### Critical

1. **ZenchanAIController -> Player:** `const GameObject* m_pTarget` and `const Box2DColliderComponent* m_pTargetBox2DCmp` — AI chases player every frame, player could die/respawn
2. **Bubble <-> Enemy (bidirectional):** `BubbleScript::m_TrappedEnemy` (ZenchanScriptComponent*) and `ZenchanScriptComponent::m_pBubble` (GameObject*) — either can die independently
3. **ZenchanBuilder -> Player:** `const GameObject* m_pTarget` — passed during Build()

### Medium
4. Game states -> GameManagerComponent (circular ownership)
5. SpriteAnimator -> active clip (pointer into map)
6. PlayerController -> PlayerScriptComponent (sibling, safe in practice)

### Why It Doesn't Crash

The destruction order happens to be correct by accident:
- Bubble `PopAndKill()` kills enemy AND destroys bubble in the same call
- Player has `Levels` persistence, enemies are `SceneLocal` — enemies die before player on transitions
- Destroy() is deferred to Cleanup() at end of frame, so mid-frame references stay valid

The game works because of how the game flows, not because the code is safe.

---

## 7. Alternatives to Global Table

| Approach | Pros | Cons |
|----------|------|------|
| **Raw pointers + discipline** | Zero overhead | Crashes if you mess up |
| **shared_ptr/weak_ptr** | Safe, standard C++ | Refcount overhead, shared ownership semantics leak into API |
| **Global table + handles** | Safe, zero-cost strong refs, cheap weak refs | Upfront infrastructure to build |
| **OnDestroy events** | Works with existing event system | Circular — needs event fix first, lots of boilerplate |
| **IsDestroyed() checks** | Simplest, works during Update() | Only safe before Cleanup() frees memory |

For 3 pointers, the pragmatic fix is `IsDestroyed()` checks + manual pointer clearing. But the global table is the "proper" solution that scales.

---

## 8. The Generation Counter

The generation counter detects stale handles after slot reuse:

```
Without generation:
1. Enemy created    -> slot[3], ptr = 0xABC
2. Handle stored    -> {index=3}
3. Enemy destroyed  -> slot[3] freed
4. Bubble created   -> slot[3] reused, ptr = 0xDEF
5. Old handle {3}   -> gets 0xDEF (WRONG OBJECT!)

With generation:
1. Enemy created    -> slot[3] = {ptr=0xABC, gen=1}
2. Handle stored    -> {index=3, gen=1}
3. Enemy destroyed  -> slot[3] = {ptr=null, gen=2}  <- bumped
4. Bubble created   -> slot[3] = {ptr=0xDEF, gen=2}
5. Old handle {3, gen=1} -> slot gen=2 != 1 -> nullptr (CORRECT)
```

The index says **where** to look, the generation says **which occupant** you expect.

---

## 9. Implementation Plan

See `Research/EngineDesignDiscussions/GlobalObjectTable_ImplementationPlan.md` and the Claude Code plan file for the full implementation plan.

### Summary

**New files:**
- `JREngine/include/JREngine/Core/ObjectHandle.h` — handle struct (index + generation)
- `JREngine/include/JREngine/Core/GlobalObjectTable.h` + `src/Core/GlobalObjectTable.cpp` — singleton registry
- `JREngine/include/JREngine/Core/WeakObjectPtr.h` — template smart pointer wrapper

**Modified files:**
- `GameObject.h/.cpp` — add ObjectHandle member, register in constructors, unregister in destructor
- `ComponentBase.h/.cpp` — same
- `CMakeLists.txt` — register new files

**Architecture:**
```
ObjectHandle {index, generation}     (trivially copyable, two uint32_t)
        |
        v
GlobalObjectTable (Singleton)        (vector<Slot> + free list)
        |
        v
WeakObjectPtr<T>                     (wraps handle, operator->/bool/Get())
```

Objects auto-register on construction, auto-unregister on destruction. `WeakObjectPtr<T>::Get()` resolves the handle through the table — returns `T*` if alive, `nullptr` if dead.
