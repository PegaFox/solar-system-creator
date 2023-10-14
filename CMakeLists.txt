cmake_minimum_required(VERSION 3.7)

project(solar_system_creator)

add_link_options(-lsfml-graphics -lsfml-window -lsfml-system)

add_executable(main main.cpp)