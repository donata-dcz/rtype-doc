cmake_minimum_required(VERSION 3.28)

set(RTYPE_VCPKG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg")
if(NOT EXISTS "${RTYPE_VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake")
    find_package(Git REQUIRED)
    message(STATUS "vcpkg not found: cloning...")
    execute_process(
        COMMAND ${GIT_EXECUTABLE} clone https://github.com/microsoft/vcpkg.git "${RTYPE_VCPKG_DIR}"
        COMMAND_ERROR_IS_FATAL ANY
    )
endif()

project(RType VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC AND VCPKG_TARGET_TRIPLET MATCHES "-static$")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/Cmake")
include(Warnings)
include(Dependencies)

add_subdirectory(Engine)
add_subdirectory(Common)
add_subdirectory(Server)
add_subdirectory(Client)
add_subdirectory(Game/RType)
add_subdirectory(Game/Pacman)

enable_testing()
add_subdirectory(Tests)
