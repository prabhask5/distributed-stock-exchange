cmake_minimum_required(VERSION 3.15)
project(MatchingEngine LANGUAGES CXX)

# Find requirements
find_package(fastcdr REQUIRED)
find_package(fastdds 3 REQUIRED)
find_package(log4cxx REQUIRED)

# C++ Standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable recommended warnings for Clang and GCC
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wconversion
        -Wsign-conversion
        -Wold-style-cast
        -Wnon-virtual-dtor
        -Woverloaded-virtual
        -Wnull-dereference
        -Wdouble-promotion
        -Wformat=2
        -Wimplicit-fallthrough
        -Wcast-align
        -Wmissing-declarations
        -Wzero-as-null-pointer-constant
        -Wuseless-cast
        -Wno-unused-parameter
    )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wconversion
        -Wsign-conversion
        -Wold-style-cast
        -Wnon-virtual-dtor
        -Woverloaded-virtual
        -Wnull-dereference
        -Wdouble-promotion
        -Wformat=2
        -Wimplicit-fallthrough=3
        -Wcast-align
        -Wmissing-declarations
        -Wzero-as-null-pointer-constant
        -Wunused
        -Wno-unused-parameter
    )
endif()

link_directories(${CMAKE_INSTALL_PREFIX}/lib)
link_directories(${QUICKFIX_INSTALL_PREFIX}/lib)

file(GLOB DISTRIBUTED_SE_MATCHING_ENGINE_SRC *)
add_executable(MatchingEngine ${DISTRIBUTED_SE_MATCHING_ENGINE_SRC})
link_directories(DistributedStockExchangeLib quickfix log4cxx boost_program_options fastdds fastcdr)
target_link_libraries(MatchingEngine DistributedStockExchangeLib quickfix log4cxx boost_program_options fastdds fastcdr)

install(TARGETS MatchingEngine PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/dse)