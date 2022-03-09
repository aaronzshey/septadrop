set(ProjectName "elnutris")

cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)

project(${ProjectName} VERSION 1.0)

file (GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(${ProjectName} ${SRC_FILES})
target_include_directories(${ProjectName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
target_link_libraries(${ProjectName} sfml-graphics)

# Generates compile_commands.json in build folder, which clangd will pick up
# Without this, clangd might assume the wrong C++ version
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)