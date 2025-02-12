cmake_minimum_required(VERSION 3.30)
project(GAME)
INCLUDE(FindPkgConfig)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

set(SDL2_PATH "C:/Users/bostj/CLionProjects/GAME/cmake-build-debug/SDL2-2.30.12/x86_64-w64-mingw32")
set(SDL2_image_PATH "C:/Users/bostj/CLionProjects/GAME/cmake-build-debug/SDL2_image-2.6.2-win32-x86" )

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
include_directories(${SLD2_INCLUDE_DIR})

add_executable(GAME src/main.cpp
        src/game.cpp
        src/game.h
        src/Texture_Manager.h
        src/Texture_Manager.cpp
        src/GameObject.h
        src/GameObject.cpp)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES})
