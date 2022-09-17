cmake_minimum_required(VERSION 3.23)
project(Hamurabi)

set(CMAKE_CXX_STANDARD 17)

add_executable(Hamurabi main.cpp
        OrderProcessor.cpp
        YearProcessor.cpp
        SaveLoadService.cpp)
