project(gtests)

add_subdirectory(lib)

include_directories(
        ${gtest_SOURCE_DIR}/include
        ${gtest_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/include
)

set(TEST_SOURCES
        ErrorHandlerTests.cpp
        OrderHandlerTests.cpp
        BufferTests.cpp
)

add_executable(gtests_run ${TEST_SOURCES})

target_link_libraries(gtests_run gtest gtest_main)

target_sources(gtests_run
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src/ErrorHandler.cpp
        ${CMAKE_SOURCE_DIR}/src/Side.cpp
        ${CMAKE_SOURCE_DIR}/src/Buffer.cpp
)

target_link_libraries(gtests_run ${Boost_LIBRARIES})
