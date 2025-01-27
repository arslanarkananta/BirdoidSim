cmake_minimum_required(VERSION 3.10)

project(3dBirdSimulation VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_TOOLCHAIN_FILE "D:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

set(RAYLIB_INCLUDE_DIR "D:/vcpkg/installed/x64-windows/include")
set(RAYLIB_LIBRARY "D:/vcpkg/installed/x64-windows/lib/raylib.lib")

add_executable(${PROJECT_NAME}
    main.cpp
    boid.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${RAYLIB_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE ${RAYLIB_LIBRARY})

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)