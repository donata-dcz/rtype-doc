add_library(Engine STATIC
    Src/GameLoop.cpp
    Src/ECS/Registry.cpp
    Src/EventBus.cpp
    Src/System/MovementSystem.cpp
    Src/System/DespawnSystem.cpp
    Src/System/BoundsSystem.cpp
)
target_include_directories(Engine
    PUBLIC
        Include
)
target_link_libraries(Engine PRIVATE RTypeWarnings)
