# The engine

The `Engine` folder is a small game engine: it only gives the tools to describe a game world and to run it at a fixed speed.

## ECS: entities, components, systems

The world is described with three ideas:

- An **entity** is only a number: `ecs::Entity`. It has no data and no code.
- A **component** is a small `struct` of data (a position, a speed, a number of lives). An entity is made of the components attached to it.
- A **system** is a class with the rules. It looks for the entities that have the components it needs and changes them.

### Components

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

Any struct is a component. A game declares its own, in its own files, and nothing has to be registered: the storage for a type is created the first time you add one.
