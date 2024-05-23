cmake_minimum_required(VERSION 3.7)

project(solar_system_creator)

add_subdirectory(${CMAKE_SOURCE_DIR}/SFML SFML)

link_libraries(sfml-graphics sfml-window sfml-system)

add_executable(main main.cpp)