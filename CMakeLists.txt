cmake_minimum_required(VERSION 3.11)


set(CMAKE_BUILD_TYPE Release)

project(pactree)

project(pactree)

execute_process(COMMAND cat /proc/cpuinfo COMMAND grep clwb OUTPUT_VARIABLE CLWB)
if(CLWB)
    message(STATUS "Use clwb")
else()
        add_definitions(-DNO_CLWB)
    message(STATUS "Use clflush instaed of clwb")
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-pthread -Wall -Wextra -march=native -mavx512bw -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -g")

include_directories(${CMAKE_SOURCE_DIR}/include)
enable_testing()

add_subdirectory(lib/PDL-ART)
add_subdirectory(src)
add_subdirectory(example)

add_library(pac SHARED pac_wrapper.cpp)
TARGET_LINK_LIBRARIES(
	pac
    pactree
    pdlart
	tbb
    numa
	pmem
	pmemobj
)

#add_subdirectory(tests)
#add_subdirectory(benchmarks)
