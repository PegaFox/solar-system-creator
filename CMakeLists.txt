cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_STANDARD 17)

project(solar_system_creator)

add_subdirectory(${CMAKE_SOURCE_DIR}/SFML SFML)

include_directories(${CMAKE_SOURCE_DIR}/SFML/include ${CMAKE_SOURCE_DIR}/glm)

link_libraries(sfml-graphics sfml-window sfml-system)

add_executable(main main.cpp)