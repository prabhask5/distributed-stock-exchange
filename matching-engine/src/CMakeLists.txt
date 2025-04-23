cmake_minimum_required(VERSION 3.15)
project(MatchingEngine LANGUAGES CXX)

# Find requirements
find_package(fastcdr REQUIRED)
find_package(fastdds REQUIRED)
find_package(log4cxx REQUIRED)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

link_directories(${CMAKE_INSTALL_PREFIX}/lib)
link_directories(${QUICKFIX_INSTALL_PREFIX}/lib)

file(GLOB DISTRIBUTED_SE_MATCHING_ENGINE_SRC *)
add_executable(MatchingEngine ${DISTRIBUTED_SE_MATCHING_ENGINE_SRC})
link_directories(DistributedSELib quickfix log4cxx boost_program_options)
target_link_libraries(MatchingEngine DistributedSELib quickfix log4cxx boost_program_options)

install(TARGETS MatchingEngine PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/dse)