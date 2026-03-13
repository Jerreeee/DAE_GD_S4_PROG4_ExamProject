# CLAUDE.md — JREngine

Static library game engine. Public API headers in `include/JREngine/`, implementation in `src/`.

## Module Layout

- **Scene/**: GameObject, Scene, SceneManager, Transform, ComponentBase, PersistenceLayer
- **Core/**: Event system, FSM/HSM state machines, ServiceLocator, Singleton, Observer, UUID, Timer, Command
- **Asset/**: ResourceManager (Runtime + Editor), AssetImporter interface, Sprite/Texture2D/Font assets, SpriteEditor
- **Input/**: InputManager, ActionMap, Binding system, SDLKeyboard, XBoxController (XInput)
- **Physics/**: BoxPhysicsSystem (AABB), RigidBody2DComponent, Box2DColliderComponent, collision layers/masks
- **Rendering/**: SDLRenderer, SpriteRendererComponent, TextRendererComponent, RendererComponentBase
- **Animation/**: SpriteAnimationClip, SpriteAnimatorComponent
- **Audio/**: ISoundSystem/ISoundClip/ISoundMusic interfaces, SDL implementations
- **Math/**: Utility math helpers

## Adding New Components

1. Create header in `include/JREngine/<Subsystem>/` and source in `src/<Subsystem>/`
2. Derive from `ComponentBase`
3. Register in `JREngine/CMakeLists.txt` (source files are listed explicitly, not globbed)

## Adding New Asset Types

1. Derive from `Asset` for the data class
2. Implement `IAssetImporter` for loading
3. Register importer with the ResourceManager

## Notes

- `FixedUPdate` (capital P) — this is a known typo in the FSM interface, keep consistent until refactored
- Physics gravity default: 40 units/sec²
- Scene renders components in registration order via `m_RendererComponents`
- Transform uses dirty flag optimization for world-space calculations
