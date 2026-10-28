# The engine

The `Engine` folder is a small game engine. It knows nothing about R-Type, about the network or about the screen: it only gives the tools to describe a game world and to run it at a fixed speed. R-Type is one game built on top of it, and [a new game can be built the same way](AddAGame.md).

| Part | Namespace | Header folder | Role |
|------|-----------|---------------|------|
| ECS | `ecs` | `ECS/` | Entities, components, and the `Registry` that holds them |
| Systems | `ecs` | `System/` | The rules that work on components, and three ready-made ones |
| Game loop | `Engine` | `Engine/GameLoop.hpp` | Calls your code at a fixed speed |
| Event bus | `Engine` | `Engine/EventBus.hpp` | Lets parts of a game talk without knowing each other |

- A **system** is a class with the rules. It looks for the entities that have the components it needs and changes them.

A ship is not a `Ship` class: it is an entity with a `Transform`, a `Velocity`, a `Hitbox`, and so on. A different game only needs different components and different systems.

### Components that come with the engine

All are in `ECS/Component.hpp`. They are plain structs, with default values.

| Component | Fields (default) | Used for |
|-----------|------------------|----------|
| `Transform` | `x` (0), `y` (0), `rotation` (0) | Position in the world |
| `Velocity` | `x` (0), `y` (0) | Speed, in world units per second |
| `Health` | `current` (100), `max` (100) | Life points |
| `Weapon` | `dammage` (100), `durability` (100) | Damage dealt and wear of a weapon |
| `Sprite` | `textureId` (empty string) | Name of the texture to draw |
| `Hitbox` | `width` (0), `height` (0) | Size of the entity |
| `Bounded` | none | A tag: "keep this entity inside the area" |

A tag is a component with no field. It only marks entities, so that a system can select them.

Any struct is a component. A game declares its own, in its own files, and nothing has to be registered: the storage for a type is created the first time you add one.
