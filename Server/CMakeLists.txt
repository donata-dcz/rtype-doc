add_executable(r-type_server
    Src/Main.cpp
    Src/Ticks.cpp
    Src/UdpServer.cpp
)
target_link_libraries(r-type_server PRIVATE Engine Common RType asio_headers RTypeWarnings)

target_include_directories(r-type_server
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Include
)