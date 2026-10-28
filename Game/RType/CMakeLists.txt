add_library(RType STATIC
    Src/Weapon.cpp
    Src/Entities.cpp
    Src/Systems.cpp
    Src/Game.cpp
    Src/Network/Messages.cpp
    Src/Input.cpp
)

target_include_directories(RType PUBLIC Include Include/Network)
target_link_libraries(RType PUBLIC Common Engine PRIVATE RTypeWarnings)

if(WIN32)
    target_link_libraries(RType PUBLIC ws2_32)
endif()
