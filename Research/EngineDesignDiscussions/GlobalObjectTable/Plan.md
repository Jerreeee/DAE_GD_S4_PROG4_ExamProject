# Plan: Global Object Table + WeakObjectPtr

## Context

Raw `GameObject*` and `Component*` pointers across objects can dangle when the target is destroyed. Current dangerous cases:
- `ZenchanAIController` -> player `GameObject*` and `Box2DColliderComponent*`
- `BubbleScript` -> `ZenchanScriptComponent*` (trapped enemy)
- `ZenchanScriptComponent` -> `GameObject*` (bubble carrying it)

Solution: A generational handle system (like Unreal's TWeakObjectPtr) where a global registry tracks all objects by index+generation, and `WeakObjectPtr<T>` provides safe access that returns nullptr for dead objects.

## Architecture

```
ObjectHandle {index, generation}     (trivially copyable, two uint32_t)
        |
        v
GlobalObjectTable (Singleton)        (vector<Slot> + free list)
        |
        v
WeakObjectPtr<T>                     (wraps handle, smart-pointer API)
```

- Objects register in constructor -> get a handle
- Objects unregister in destructor -> generation bumped, slot recycled
- `WeakObjectPtr<T>::Get()` resolves handle -> returns `T*` or `nullptr`

## New Files

### 1. `JREngine/include/JREngine/Core/ObjectHandle.h` (header-only)

```cpp
struct ObjectHandle
{
    uint32_t m_Index{ 0 };
    uint32_t m_Generation{ 0 };
    bool IsValid() const { return m_Generation != 0; }
    bool operator==(const ObjectHandle&) const = default;
    static const ObjectHandle Invalid;  // {0, 0}
};
```

Generation 0 = invalid. Valid generations start at 1.

### 2. `JREngine/include/JREngine/Core/GlobalObjectTable.h` + `src/Core/GlobalObjectTable.cpp`

Singleton with a `vector<Slot>` and a free list:

```cpp
class GlobalObjectTable final : public Singleton<GlobalObjectTable>
{
    friend class Singleton<GlobalObjectTable>;
public:
    ObjectHandle Register(void* pObject);
    void Unregister(ObjectHandle handle);
    void* Resolve(ObjectHandle handle) const;
private:
    GlobalObjectTable() = default;
    struct Slot
    {
        void* m_pObject{ nullptr };
        uint32_t m_Generation{ 1 };
        bool m_Occupied{ false };
        uint32_t m_NextFree{ UINT32_MAX };
    };
    std::vector<Slot> m_Slots;
    uint32_t m_FreeListHead{ UINT32_MAX };
};
```

- **Register:** Pop from free list (or push_back new slot). Set pointer, mark occupied, return `{index, generation}`.
- **Unregister:** Validate handle, null pointer, bump generation, mark unoccupied, push onto free list.
- **Resolve:** Bounds check + generation match + occupied check -> return pointer or nullptr.

Also define `ObjectHandle::Invalid` in the .cpp.

### 3. `JREngine/include/JREngine/Core/WeakObjectPtr.h` (header-only template)

```cpp
template<typename T>
class WeakObjectPtr
{
public:
    WeakObjectPtr() = default;
    WeakObjectPtr(std::nullptr_t) {}
    explicit WeakObjectPtr(T* pObject);        // reads handle from pObject->GetObjectHandle()
    explicit WeakObjectPtr(ObjectHandle handle);

    T* Get() const;                // Resolve -> static_cast<T*> or nullptr
    explicit operator bool() const;
    T* operator->() const;         // assert + Get()
    T& operator*() const;          // assert + Get()
    ObjectHandle GetHandle() const;

    WeakObjectPtr& operator=(std::nullptr_t);
    bool operator==(const WeakObjectPtr&) const = default;
    bool operator==(std::nullptr_t) const;

private:
    ObjectHandle m_Handle{};
};
```

## Modified Files

### 4. `JREngine/include/JREngine/Scene/GameObject.h`

- Add `#include "JREngine/Core/ObjectHandle.h"`
- Add member: `ObjectHandle m_ObjectHandle{};` (in private section, ~line 132)
- Add public accessor: `ObjectHandle GetObjectHandle() const { return m_ObjectHandle; }`
- Change destructor from `~GameObject() = default;` to `~GameObject();` (definition moves to .cpp)

### 5. `JREngine/src/Scene/GameObject.cpp`

- Add `#include "Core/GlobalObjectTable.h"`
- In default constructor: add body with `m_ObjectHandle = GlobalObjectTable::GetInstance().Register(this);`
- In named constructor: same registration line
- Add destructor: `GameObject::~GameObject() { GlobalObjectTable::GetInstance().Unregister(m_ObjectHandle); }`

### 6. `JREngine/include/JREngine/Scene/ComponentBase.h`

- Add `#include "JREngine/Core/ObjectHandle.h"`
- Add member: `ObjectHandle m_ObjectHandle{};` (in private section, ~line 37)
- Add public accessor: `ObjectHandle GetObjectHandle() const { return m_ObjectHandle; }`
- Change destructor from `virtual ~ComponentBase() = default;` to `virtual ~ComponentBase();`

### 7. `JREngine/src/Scene/ComponentBase.cpp`

- Add `#include "Core/GlobalObjectTable.h"`
- In constructor: add `m_ObjectHandle = GlobalObjectTable::GetInstance().Register(this);`
- Add destructor: `ComponentBase::~ComponentBase() { GlobalObjectTable::GetInstance().Unregister(m_ObjectHandle); }`

### 8. `JREngine/CMakeLists.txt`

SOURCE_FILES (~line 18, after Core/ block):
```
"Core/GlobalObjectTable.cpp"
```

INCLUDE_FILES (~line 82, after Core/ block):
```
"Core/ObjectHandle.h"
"Core/GlobalObjectTable.h"
"Core/WeakObjectPtr.h"
```

## Implementation Order

1. **ObjectHandle.h** -- standalone struct, no dependencies
2. **GlobalObjectTable.h + .cpp** -- depends on ObjectHandle + Singleton
3. **WeakObjectPtr.h** -- depends on ObjectHandle + GlobalObjectTable
4. **ComponentBase.h/.cpp** -- add handle member, register/unregister
5. **GameObject.h/.cpp** -- add handle member, register/unregister
6. **CMakeLists.txt** -- register new files
7. **Build + verify** -- should compile clean, no behavior change yet

Game-side migration (replacing raw pointers with WeakObjectPtr) is a separate step after the infrastructure is in.

## Verification

1. `cmake --preset x64-debug && cmake --build out/build/x64-debug` -- must compile clean with /W4 /WX
2. Run `./out/build/x64-debug/bin/BubbleBobble.exe` -- game should behave identically (no functional change yet, just registration happening behind the scenes)
3. Verify no crashes on scene transitions (objects registering/unregistering correctly)
4. Verify no crashes on game exit (GlobalObjectTable singleton destroyed after all scenes)
