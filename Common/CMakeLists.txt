add_library(
Common
    STATIC
        Src/Network/Binary.cpp
        Src/Network/Protocol.cpp
)

target_include_directories(Common PUBLIC Include Include/Network)
target_link_libraries(Common PUBLIC Engine PRIVATE RTypeWarnings)

if(WIN32)
    target_link_libraries(Common PUBLIC ws2_32)
endif()
