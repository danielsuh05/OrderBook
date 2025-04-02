cmake_minimum_required(VERSION 3.27)
project(SimpleOrderBook)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(BOOST_ROOT "/opt/homebrew/opt/boost")
set(Boost_NO_SYSTEM_PATHS TRUE)
find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

include_directories(include)

set(SOURCES
        src/main.cpp
        src/ITCHParser.cpp
        src/ErrorHandler.cpp
        src/ITCHMessageType.cpp
)

set(HEADERS
        include/ITCHParser.h
        include/Order.h
        include/Side.h
        include/ITCHOrderType.h
        include/Error.h
        include/ErrorHandler.h
        include/ErrorType.h
        include/ITCHMessageType.h
)

add_executable(SimpleOrderBook ${SOURCES} ${HEADERS})

target_link_libraries(SimpleOrderBook ${Boost_LIBRARIES})

add_subdirectory(gtests)
