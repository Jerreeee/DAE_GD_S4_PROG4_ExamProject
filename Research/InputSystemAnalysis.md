# Input System — Complete Analysis

## Architecture Overview

The input system follows a layered design: **Devices** (poll hardware) → **Bindings** (map input to conditions) → **ActionMaps** (group bindings per context) → **Commands** (execute game logic). Managed by `InputManager` singleton.

## Class Hierarchy

```
IInputDevice (InputUtils.h)
├── IKeyboard (IKeyboard.h)
│   └── SDLKeyboard (src/Input/SDLKeyboard.h/.cpp) — pimpl pattern
└── IController (IController.h)
    └── XBoxController (src/Input/XBoxController.h/.cpp) — pimpl pattern

IBindingInfo (InputUtils.h)
├── KeyboardBindingInfo (IKeyboard.h) — stores KeyboardKey + KeyState
└── ControllerBindingInfo (IController.h) — stores ControllerButton + ButtonState
```

## Enumerations (InputUtils.h)

**DeviceType**: `Keyboard = 0`, `Controller = 1` — used as array indices in ActionMap.devicesInfo

**KeyState / ButtonState** (same enum, aliased):
- `Up` — not pressed (always returns true — see issues)
- `DownThisFrame` — just pressed this frame
- `UpThisFrame` — just released this frame
- `Pressed` — currently held down

**KeyboardKey** (uint8_t): A-Z (0-25), F1-F2 (26-27), MAX = 28

**ControllerButton** (uint8_t): DPAD directions, START, BACK, thumbstick clicks, shoulder buttons, face buttons (A/B/X/Y)

## Core Data Structures (InputManager.h)

### Command (Core/Command.h)
```cpp
using Command = std::function<void()>;
```

### DeviceInfo
```cpp
struct DeviceInfo {
    std::unique_ptr<IInputDevice> pDevice;
    bool polledThisTick;  // optimization flag (currently broken — see issues)
};
```

### Binding
```cpp
struct Binding {
    std::unique_ptr<IBindingInfo> pBindInfo;  // what input to listen for
    Command command;                           // what to execute (can be nullptr)
};
```

### ActionMap
```cpp
struct ActionMap {
    bool m_Enabled;
    std::array<DeviceInfo*, 2> devicesInfo;            // [keyboard, controller]
    std::map<std::string, Binding> m_Bindings;         // named bindings
};
```

## InputManager (Singleton)

### Public API
| Method | Description |
|--------|-------------|
| `ProcessInput()` | Main per-frame loop: polls SDL events, polls devices, executes commands. Returns false on SDL_QUIT |
| `AddKeyboard()` | Creates SDLKeyboard, returns device index |
| `AddController()` | Creates XBoxController (auto-incrementing index), returns device index |
| `AddActionMap(deviceIndices)` | Creates ActionMap with assigned devices, returns ActionMap index |
| `BindCommand(mapIdx, name, command, bindingInfo)` | Registers binding, returns `*this` for chaining |
| `IsBindingActive(actionMap, name)` | Checks if a specific named binding is active right now |
| `SetEnableActionMap(mapIdx, enable)` | Toggles an ActionMap on/off |
| `GetActionMap(mapIdx)` | Returns const reference to ActionMap |

### Private Data
- `m_Devices` — vector of all DeviceInfo (keyboards + controllers)
- `m_ActionMaps` — vector of all ActionMaps
- `m_NrControllers` — counter for controller indices
- `m_PolledDevices` — vector<bool>, declared but never actually used

## Device Implementation Details

### SDLKeyboard (pimpl)
- Uses `SDL_GetKeyboardState()` for passive state polling (not event-driven)
- Only tracks keys registered via `AddKeysToTrack()` (optimization)
- Maps `KeyboardKey` enum → `SDL_Scancode` via internal `MapToInternalKey()`
- State tracking via bitsets:
  - `m_CurrentState` / `m_PreviousState` — bitset<MAX_NR_KEYBOARD_KEYS>
  - `m_ButtonChanges` = current XOR previous
  - `m_ButtonsPressedThisFrame` = changes AND current
  - `m_ButtonsReleasedThisFrame` = changes AND NOT current

### XBoxController (pimpl)
- Uses Windows XInput API (`XInputGetState()`)
- Constructor takes controller index (0-3)
- Polls all buttons every frame (no selective tracking like keyboard)
- Same bitset logic as keyboard but on `XINPUT_GAMEPAD.wButtons` WORD
- No analog trigger/stick support — only digital button states
- Maps `ControllerButton` enum → XInput constants via static array

## Processing Flow (per frame)

```
InputManager::ProcessInput()
│
├─ 1. SDL_PollEvent loop
│     ├─ SDL_QUIT → return false (engine quits)
│     ├─ SDL_KEYDOWN → (empty handler)
│     ├─ SDL_MOUSEBUTTONDOWN → (empty handler)
│     └─ ImGui_ImplSDL2_ProcessEvent(&e)
│
├─ 2. Reset m_PolledDevices (fills with false — but vector is empty/unused)
│
├─ 3. For each enabled ActionMap:
│     ├─ Poll devices (if not polledThisTick — but flag is never set, so always polls)
│     │   ├─ SDLKeyboard::PollState() → SDL_GetKeyboardState + bitset math
│     │   └─ XBoxController::PollState() → XInputGetState + bitset math
│     │
│     └─ For each Binding:
│         ├─ Skip if command is nullptr
│         ├─ Get device via bindInfo->GetType() as index
│         ├─ device->IsBindingActive(bindInfo) → HasKeyState/HasButtonState
│         └─ If active → execute command()
│
└─ return true (continue running)
```

## Game Usage (BubbleBobble)

### Device Setup (GameManagerComponent.cpp)
```cpp
auto& im = InputManager::GetInstance();
im.AddController();   // device index 0
im.AddKeyboard();     // device index 1
```

### Main Menu (MainMenuState.cpp)
- ActionMap with controller only: `AddActionMap({ 0 })`
- DPAD_LEFT → StartSinglePlayer, DPAD_RIGHT → StartMultiPlayer, DPAD_UP → StartVersus
- Disabled by default, enabled on menu enter, disabled on menu exit

### In-Game (InGameState.cpp)
- P1 ActionMap with both devices: `AddActionMap({ 0, 1 })`
- F1 key → SkipLevel (keyboard, with command lambda)
- DPAD_LEFT/RIGHT → MoveLeft/MoveRight (controller, **nullptr command** — polled via IsBindingActive)
- FACE_DOWN → Jump, FACE_LEFT → Shoot (controller, nullptr commands)
- P2 ActionMap with keyboard only (multiplayer): MoveLeft/Right on A/D, Jump on W, Shoot on E

### Player Controller Pattern (PlayerController.cpp)
Two usage patterns coexist:
1. **Command execution** — BindCommand with a lambda, executed automatically by InputManager
2. **Manual polling** — BindCommand with nullptr, then `IsBindingActive()` in PlayerControllerComponent::Update()

```cpp
void PlayerControllerComponent::Update() {
    auto& input = InputManager::GetInstance();
    if (input.IsBindingActive(*m_pActionMap, "MoveLeft"))
        m_pPlayerScript->Move(-1);
    else if (input.IsBindingActive(*m_pActionMap, "MoveRight"))
        m_pPlayerScript->Move(1);
    else
        m_pPlayerScript->Move(0);

    if (input.IsBindingActive(*m_pActionMap, "Jump"))
        m_pPlayerScript->Jump();
    if (input.IsBindingActive(*m_pActionMap, "Shoot"))
        m_pPlayerScript->Shoot();
}
```

### Game Over (GameOverScreenState.cpp)
- ActionMap with controller only
- DPAD_LEFT → ReturnToMainMenu

### State Transitions
Each game state enables/disables its ActionMap on enter/exit:
```cpp
void MainMenuState::OnEnter() {
    InputManager::GetInstance().SetEnableActionMap(m_ActionMapIdx, true);
}
void MainMenuState::OnExit() {
    InputManager::GetInstance().SetEnableActionMap(m_ActionMapIdx, false);
}
```

## Design Patterns

1. **Singleton** — InputManager via `Singleton<T>` template
2. **Pimpl** — SDLKeyboard::Impl, XBoxController::Impl hide platform details
3. **Strategy** — IInputDevice polymorphism for different device types
4. **Command** — `std::function<void()>` for deferred/configurable actions
5. **Factory** — `AddKeyboard()`, `AddController()` create concrete devices

## What's Good

- **Selective key tracking** — SDLKeyboard only polls registered keys, efficient for large keyboards
- **Bitset state tracking** — efficient frame-to-frame change detection
- **ActionMap enable/disable** — clean context switching between menu/gameplay/etc.
- **Fluent API** — `BindCommand()` returns `*this` for chaining
- **Named bindings** — string names allow `IsBindingActive()` queries without storing command references
- **Pimpl on devices** — platform details (SDL, XInput) hidden from headers

## Issues Found

### Bug: polledThisTick Never Set to True
In `InputManager.cpp` line 41-42, `PollState()` is called but `polledThisTick` is never set to `true`. The optimization to prevent double-polling the same device across multiple ActionMaps doesn't work. Devices sharing multiple ActionMaps get polled multiple times per frame.

Additionally, `m_PolledDevices` (vector<bool>) is declared and reset each frame but never actually read or written meaningfully — it's dead code.

### Bug: KeyState::Up Always Returns True
`IKeyboard::HasKeyState()` with `KeyState::Up` returns `true` unconditionally. A binding with Up state would always fire. Same for `IController::HasButtonState()`.

### Issue: Two Competing Input Patterns
The system supports both automatic command execution (lambda in BindCommand) and manual polling (nullptr command + IsBindingActive). This creates confusion:
- Menu states use command lambdas (fire-and-forget)
- Player controller uses nullptr commands + manual polling in Update()
- No clear guidance on which pattern to use when

### Issue: Empty SDL Event Handlers
SDL_KEYDOWN and SDL_MOUSEBUTTONDOWN handlers in ProcessInput() are empty. Keyboard input works via `SDL_GetKeyboardState()` polling, not events. The event handlers seem like leftover scaffolding.

### Issue: No Analog Input Support
XBoxController only tracks digital button states. No support for:
- Analog triggers (LT/RT pressure)
- Analog thumbsticks (direction + magnitude)
- Deadzone handling

### Issue: Limited Keyboard Keys
Only A-Z and F1-F2 are supported (28 keys total). Missing: arrow keys, space, enter, escape, shift, ctrl, numbers, etc. This severely limits keyboard input options.

### Issue: No Input Rebinding
Bindings are hardcoded at setup time. No runtime rebinding, no serialization/deserialization of key mappings, no settings menu support.

### Issue: ActionMaps Never Removed
ActionMaps are created but never destroyed. Over many game state transitions, the ActionMap vector grows (though disabled maps have minimal cost). No cleanup mechanism exists.

### Issue: No Mouse Support
Despite having an empty SDL_MOUSEBUTTONDOWN handler, there's no mouse device, no mouse bindings, no cursor position tracking.

## File Inventory

### Engine Headers (public)
- `JREngine/include/JREngine/Input/InputManager.h`
- `JREngine/include/JREngine/Input/InputUtils.h`
- `JREngine/include/JREngine/Input/IKeyboard.h`
- `JREngine/include/JREngine/Input/IController.h`
- `JREngine/include/JREngine/Input/ActionMap.h` (empty — ActionMap defined in InputManager.h)
- `JREngine/include/JREngine/Core/Command.h`
- `JREngine/include/JREngine/Core/Singleton.h`

### Engine Sources (private)
- `JREngine/src/Input/InputManager.cpp`
- `JREngine/src/Input/SDLKeyboard.h` + `SDLKeyboard.cpp`
- `JREngine/src/Input/XBoxController.h` + `XBoxController.cpp`

### Game Usage
- `BubbleBobble/src/GameManager/GameManagerComponent.cpp` — device creation
- `BubbleBobble/src/GameManager/MainMenuState.cpp` — menu bindings
- `BubbleBobble/src/GameManager/InGameState.cpp` — gameplay bindings
- `BubbleBobble/src/GameManager/GameOverScreenState.cpp` — game over bindings
- `BubbleBobble/src/Player/PlayerController.h/.cpp` — manual binding polling
- `BubbleBobble/src/Player/PlayerScriptComponent.h/.cpp` — receives input commands
- `BubbleBobble/src/Player/PlayerBuilder.h/.cpp` — player factory with ActionMap index

---

## Suggestions for Improvement

### Priority 1: Fix polledThisTick Bug
The `polledThisTick` flag is never set to `true` after polling, so devices shared across multiple ActionMaps get polled multiple times per frame. Fix: set `polledThisTick = true` after the `PollState()` call, and reset all flags at the start of `ProcessInput()`. Also remove the unused `m_PolledDevices` vector — it's dead code.

### Priority 2: Fix KeyState::Up Always Returning True
`HasKeyState(KeyState::Up)` / `HasButtonState(ButtonState::Up)` always returns `true`. The "Up" state should check that the key is NOT currently pressed:
```cpp
case KeyState::Up:
    return !m_CurrentState[keyIndex];
```
Without this fix, any binding using the `Up` state fires every single frame.

### Priority 3: Pick One Input Pattern
Two competing patterns exist — command lambdas (auto-fire) and nullptr commands with `IsBindingActive()` polling. Standardize on one:
- **Recommendation**: Use command lambdas for instant actions (menu navigation, jump, shoot) and polling for continuous actions (movement direction). Document this convention.
- Alternatively, unify into a single approach: always use polling, and remove the auto-execute path. This makes the flow more predictable — game code always decides when to act.

### Priority 4: Expand Keyboard Key Support
Only 28 keys (A-Z, F1-F2) are supported. Add at minimum:
- Arrow keys, Space, Enter, Escape, Shift, Ctrl, Tab
- Number keys 0-9
- Common punctuation (period, comma, etc.)

This is straightforward — just extend the `KeyboardKey` enum and the `MapToInternalKey()` SDL scancode mapping table.

### Priority 5: Add Analog Stick / Trigger Support
`XBoxController` only reads digital button states. For proper controller support, add:
- Left/right thumbstick axes (X, Y) as float values (-1.0 to 1.0)
- Left/right trigger pressure (0.0 to 1.0)
- Configurable deadzone thresholds

This would require extending `IController` with axis query methods and adding a new `AxisBinding` type alongside the existing button bindings.

### Priority 6: ActionMap Cleanup
ActionMaps are created but never destroyed. Add:
- `RemoveActionMap(idx)` — destroys the ActionMap and its bindings
- Or: automatic cleanup when a game state is destroyed (tie ActionMap lifetime to the state that created it)

For current scope this is low-priority since disabled ActionMaps have near-zero per-frame cost.

### Priority 7: Add Mouse Device
The scaffolding already exists (empty `SDL_MOUSEBUTTONDOWN` handler). Implement:
- `MouseDevice` implementing `IInputDevice`
- Track button states (left, right, middle) + cursor position + scroll wheel
- `MouseBindingInfo` for button bindings
- Expose cursor position via `GetMousePosition()` on InputManager

### Priority 8: Runtime Rebinding Support
Currently all bindings are hardcoded at setup time. For a settings menu:
- Allow changing the `IBindingInfo` on an existing named binding
- Serialize/deserialize bindings to/from a config file
- This builds on the named binding system — the infrastructure is already there, just needs a `RebindCommand()` method

### Priority 9: Remove Empty SDL Event Handlers
The `SDL_KEYDOWN` and `SDL_MOUSEBUTTONDOWN` cases in `ProcessInput()` are empty — keyboard works via `SDL_GetKeyboardState()` polling, not events. Remove the dead handlers to avoid confusion, or implement them if event-driven input is desired.
