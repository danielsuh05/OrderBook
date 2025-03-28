cmake_minimum_required(VERSION 3.27)
project(SimpleOrderBook)

set(CMAKE_CXX_STANDARD 20)

add_executable(SimpleOrderBook main.cpp
        OrderParser.cpp
        OrderParser.h
        Order.cpp
        Order.h
        Side.h
        OrderType.h
        testing/strtok_vs_string_view.cpp)
