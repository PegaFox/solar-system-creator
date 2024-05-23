cmake_minimum_required(VERSION 3.7)

project(solar_system_creator)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

link_libraries(sfml-graphics sfml-window sfml-system)

add_executable(main main.cpp)