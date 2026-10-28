add_library(PacmanCore STATIC Src/PacmanGame.cpp Src/Systems.cpp)
target_include_directories(PacmanCore PUBLIC Include)
target_link_libraries(PacmanCore PUBLIC Engine PRIVATE RTypeWarnings)

add_executable(pacman_game Src/Main.cpp)
target_link_libraries(pacman_game PRIVATE PacmanCore raylib glfw RTypeWarnings)

if(WIN32)
    target_link_libraries(pacman_game PRIVATE winmm)
endif()

add_custom_target(PacmanAssets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/Assets $<TARGET_FILE_DIR:pacman_game>/Assets
    COMMENT "Copying assets next to pacman_game")