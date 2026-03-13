# Plan: Implement Layer System for JREngine

## Context

The engine currently has a monolithic `RunOneFrame()` that directly calls singletons in a fixed order: InputManager → SceneManager::FixedUpdate → Physics → SceneManager::Update → Cleanup → SDLRenderer::Render. There's no abstraction for grouping or ordering system-level functionality. A Layer system introduces a clean separation between engine concerns, game logic, and overlays, making the architecture extensible.

## Design Decisions

- **Flat stack, no sublayers** — sublayers add recursive complexity with little benefit at this scale
- **Existing singletons stay** — layers call into SceneManager/InputManager/etc. rather than replacing them
- **Event propagation: top-to-bottom** (overlays first) — ImGui overlay consumes mouse/keyboard before game sees it
- **Resource Management stays in ServiceLocator** — no separate layer, it has no per-frame work to do

## Layer Base Class

New file: `JREngine/include/JREngine/Core/Layer.h`

```cpp
class Layer {
    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate() {}
    virtual void OnFixedUpdate() {}
    virtual void OnRender() {}
    virtual void OnImGuiRender() {}
    virtual bool OnEvent(SDL_Event& event) { return false; } // true = consumed

    void SetEnabled(bool); bool IsEnabled() const;
    const std::string& GetName() const;
};
```

## LayerStack

New files: `JREngine/include/JREngine/Core/LayerStack.h`, `JREngine/src/Core/LayerStack.cpp`

- Vector split into two zones: `[0, insertIndex)` = regular layers, `[insertIndex, end)` = overlays
- `PushLayer()` inserts at insertIndex (increments it)
- `PushOverlay()` appends to back
- `PopLayer()` / `PopOverlay()` remove and call OnDetach
- Supports forward iteration (update/render: bottom→top) and reverse (events: top→bottom)

## Built-in Layers

### EngineLayer
New files: `JREngine/include/JREngine/Core/EngineLayer.h`, `JREngine/src/Core/EngineLayer.cpp`

```
OnFixedUpdate(): SceneManager::FixedUpdate() + PhysicsSystem::Update()
OnUpdate():      SceneManager::Update() + SceneManager::Cleanup()
OnRender():      Iterates Scene::GetRenderComponents() and calls Render()
OnImGuiRender(): Iterates render components and calls UpdateAndRenderUI()
```

### ImGuiOverlayLayer
New files: `JREngine/include/JREngine/Core/ImGuiOverlayLayer.h`, `JREngine/src/Core/ImGuiOverlayLayer.cpp`

```
OnEvent(): Checks ImGui::GetIO().WantCaptureMouse/Keyboard → returns true to consume
OnImGuiRender(): Engine-level debug UI (can be empty initially)
```

Pushed as overlay so it renders last and gets events first.

## Game Layer (BubbleBobble)

New files: `BubbleBobble/src/BubbleBobbleLayer.h`, `BubbleBobble/src/BubbleBobbleLayer.cpp`

```
OnAttach(): Everything currently in BubbleBobble::load() moves here
OnDetach(): Cleanup
OnImGuiRender(): Game-specific debug UI (optional)
```

## Refactored Main Loop (`RunOneFrame`)

```
1. Calculate deltaTime (unchanged)
2. SDL_PollEvent loop (moved FROM InputManager TO here)
   - ImGui_ImplSDL2_ProcessEvent on each event
   - Check SDL_QUIT
   - Propagate through layers in REVERSE order (overlays first)
   - Stop propagation if layer returns true (consumed)
3. InputManager::ProcessInputBindings() (new method — polls devices + executes commands only)
4. Fixed update loop: iterate layers forward, call OnFixedUpdate()
5. Variable update: iterate layers forward, call OnUpdate()
6. Render: SDL_RenderClear + ImGui NewFrame
7. Iterate layers forward, call OnRender()
8. Iterate layers forward, call OnImGuiRender()
9. ImGui::Render() + SDL_RenderPresent
```

## New `Main.cpp` Pattern

```cpp
JRE::JREngine engine(data_location);
engine.PushLayer(std::make_unique<JRE::EngineLayer>());
engine.PushLayer(std::make_unique<BubbleBobbleLayer>());
engine.PushOverlay(std::make_unique<JRE::ImGuiOverlayLayer>());
engine.Run();  // no callback — layers handle setup via OnAttach
```

## Files to Modify

| File | Change |
|------|--------|
| `JREngine/include/JREngine/JREngine.h` | Add `LayerStack m_LayerStack`, `PushLayer()`, `PushOverlay()`, change `Run()` signature |
| `JREngine/src/JREngine.cpp` | Refactor `RunOneFrame()` to iterate layers, `Run()` calls OnAttach/OnDetach, move SDL event loop here |
| `JREngine/include/JREngine/Input/InputManager.h` | Add `ProcessInputBindings()` declaration |
| `JREngine/src/Input/InputManager.cpp` | Extract binding processing from `ProcessInput()` into `ProcessInputBindings()` |
| `JREngine/src/Rendering/SDLRenderer.cpp` | Remove scene rendering + ImGui frame management from `Render()` (keep utility methods) |
| `JREngine/include/JREngine/Rendering/SDLRenderer.h` | Remove or simplify `Render()` |
| `JREngine/CMakeLists.txt` | Add 6 new source/header files |
| `BubbleBobble/src/Main.cpp` | Switch to layer-push pattern |
| `BubbleBobble/CMakeLists.txt` | Add BubbleBobbleLayer files |

## New Files to Create

| File | Purpose |
|------|---------|
| `JREngine/include/JREngine/Core/Layer.h` | Layer base class (header-only) |
| `JREngine/include/JREngine/Core/LayerStack.h` | LayerStack header |
| `JREngine/src/Core/LayerStack.cpp` | LayerStack implementation |
| `JREngine/include/JREngine/Core/EngineLayer.h` | Engine layer header |
| `JREngine/src/Core/EngineLayer.cpp` | Engine layer implementation |
| `JREngine/include/JREngine/Core/ImGuiOverlayLayer.h` | ImGui overlay header |
| `JREngine/src/Core/ImGuiOverlayLayer.cpp` | ImGui overlay implementation |
| `BubbleBobble/src/BubbleBobbleLayer.h` | Game layer header |
| `BubbleBobble/src/BubbleBobbleLayer.cpp` | Game layer implementation |

## Implementation Order

1. `Layer.h` — no dependencies, standalone
2. `LayerStack.h/.cpp` — depends only on Layer
3. Update `JREngine.h` — add LayerStack member and push methods
4. Split `InputManager::ProcessInput()` → keep old method, add `ProcessInputBindings()`
5. `EngineLayer` — wraps existing SceneManager/Physics calls
6. `ImGuiOverlayLayer` — wraps ImGui event consumption
7. Refactor `SDLRenderer::Render()` — remove orchestration, keep rendering utilities
8. Refactor `RunOneFrame()` — iterate layers instead of calling systems directly
9. Refactor `Run()` — OnAttach/OnDetach lifecycle, remove callback parameter
10. `BubbleBobbleLayer` — move `load()` into OnAttach
11. Update `Main.cpp` — push layers, call `engine.Run()`
12. Update both `CMakeLists.txt` — register all new files
13. Build and test

Steps 1-3 are additive (nothing breaks). Steps 4-8 prepare subsystems. Step 8-9 is the switchover. Steps 10-11 migrate the game.

## Verification

1. `cmake --build out/build/x64-debug` — must compile cleanly with /W4 /WX
2. Run `BubbleBobble.exe` — game must behave identically (main menu loads, can navigate, play, etc.)
3. Verify ImGui overlay still renders on top of game content
4. Verify ImGui consumes mouse/keyboard when focused (doesn't pass to game)

---

## Suggestions for Improvement

### Priority 1: Layer Enable/Disable Should Skip All Callbacks
When `SetEnabled(false)` is called, ensure that `OnUpdate()`, `OnFixedUpdate()`, `OnRender()`, and `OnImGuiRender()` are all skipped — but `OnEvent()` should still be called so layers can handle reactivation triggers. Document this behavior clearly, as disabling a layer mid-frame could cause subtle bugs if some callbacks already ran.

### Priority 2: Layer Communication / Dependencies
Layers currently have no way to communicate with each other. Consider:
- A shared `LayerContext` struct passed to all callbacks, containing frame timing, input state, etc.
- Or: let layers query the stack for other layers by type (e.g., `stack.GetLayer<EngineLayer>()`)
- Keep it simple — avoid a full message-passing system between layers

### Priority 3: Consider a DebugLayer
Beyond ImGuiOverlayLayer, a dedicated DebugLayer could provide:
- FPS counter, frame time graph
- Entity/component inspector (iterate SceneManager)
- Physics debug rendering (draw collider outlines)
- Toggle with a hotkey (F3 or similar)

This is a natural extension once the layer system is in place and keeps debug code cleanly separated from game logic.

### Priority 4: Event Consumption Should Be Granular
The current plan has `OnEvent()` returning a single bool (consumed or not). Consider returning an enum or struct indicating which part of the event was consumed:
```cpp
enum class EventResult { Ignored, ConsumedMouse, ConsumedKeyboard, ConsumedAll };
```
This allows ImGui to consume mouse events while still letting keyboard events pass through to the game layer — important for gameplay where mouse controls the UI but keyboard controls the character.

### Priority 5: Layer Ordering Guarantees
The current plan uses a simple insert-index split (layers vs overlays). If more ordering control is needed later, consider:
- Integer priority values on layers (lower = updates first)
- Named dependency declarations (e.g., "this layer must run after EngineLayer")

For now the two-zone approach is sufficient — don't over-engineer unless ordering becomes a real problem.

### Priority 6: Hot-Reloading Layers
Once layers are in place, it becomes possible to detach and re-attach a game layer without restarting the engine. This enables:
- Scene reloading by detaching/re-attaching BubbleBobbleLayer
- Faster iteration during development
- Only worth implementing if iteration speed becomes a bottleneck

### Priority 7: Consider Moving InputManager Processing Into a Layer
The plan keeps `InputManager::ProcessInputBindings()` as a separate step in `RunOneFrame()`. An alternative is to make input processing a layer itself (`InputLayer`), which would:
- Make the entire frame loop just "iterate layers"
- Allow input processing order to be controlled via layer priority
- Keep the architecture consistent (everything is a layer)

Trade-off: adds another layer for minimal benefit at this scale. Only worth it if the input system grows significantly.
