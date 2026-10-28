add_library(ClientCore
    STATIC
        Src/Starfield.cpp
        Src/ShipAnimation.cpp
)

target_include_directories(ClientCore PUBLIC Include)
target_link_libraries(ClientCore PRIVATE RTypeWarnings)

add_executable(r-type_client
    Src/Main.cpp
    Src/StarfieldRenderer.cpp
    Src/ResourceManager.cpp
    Src/Keyboard.cpp
    Src/SnapshotRenderer.cpp
    Src/NetworkClient.cpp
    Src/ServerConnection.cpp
    Src/Menu.cpp
    Src/MenuRenderer.cpp
    Src/Audio.cpp
)

target_link_libraries(r-type_client
    PRIVATE
        ClientCore
        RType
        Engine
        Common
        raylib
        glfw
        asio_headers
        RTypeWarnings
)

if(WIN32)
    target_link_libraries(r-type_client PRIVATE winmm)
endif()

# Copy the assets next to the executable
add_custom_target(ClientAssets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/Assets $<TARGET_FILE_DIR:r-type_client>/Assets
    COMMENT "Copying the assets next to r-type_client")
