# gtests/CMakeLists.txt

# Declare this as a subproject
project(gtests)

# Add Google Test
add_subdirectory(lib)

# Include headers from main project and Google Test
include_directories(
        ${gtest_SOURCE_DIR}/include
        ${gtest_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/include  # Use CMAKE_SOURCE_DIR instead of PROJECT_SOURCE_DIR
)

# List test source files
set(TEST_SOURCES
        OrderParserTests.cpp
)

# Create test executable
add_executable(gtests_run ${TEST_SOURCES})

# Link with gtest libraries
target_link_libraries(gtests_run gtest gtest_main)

# Also link with your main project’s compiled object files
target_sources(gtests_run
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src/ErrorHandler.cpp
        ${CMAKE_SOURCE_DIR}/src/ITCHParser.cpp
)

# Optional: link with Boost or others, if needed
target_link_libraries(gtests_run ${Boost_LIBRARIES})
