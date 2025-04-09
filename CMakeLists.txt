cmake_minimum_required(VERSION 3.27)
project(SimpleOrderBook)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(BOOST_ROOT "/opt/homebrew/opt/boost")
set(Boost_NO_SYSTEM_PATHS TRUE)

#add_definitions(-DDEBUG)

find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

include_directories(include)

set(SOURCES
        src/main.cpp
        src/OrderBookManager.cpp
        src/ErrorHandler.cpp
        src/Side.cpp
        src/Buffer.cpp
        src/ITCHParser.cpp
        src/OrderBook.cpp
)

set(HEADERS
        include/OrderBookManager.h
        include/OrderMap.h
        include/Pool.h
        include/Order.h
        include/LevelPtr.h
        include/Level.h
        include/ITCHOrder.h
        include/OrderBook.h
        include/ITCHParser.h
        include/Side.h
        include/Buffer.h
        include/ITCHOrder.tpp
        include/Error.h
        include/ErrorHandler.h
        include/ErrorType.h
        include/ITCHMessageType.h
        include/Constants.h
)

add_executable(SimpleOrderBook ${SOURCES} ${HEADERS})

target_link_libraries(SimpleOrderBook ${Boost_LIBRARIES})

add_subdirectory(gtests)
