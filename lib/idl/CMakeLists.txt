cmake_minimum_required(VERSION 3.15)
project(DistributedStockExchangeLib LANGUAGES CXX)

# Find requirements
find_package(fastcdr REQUIRED)
find_package(fastrtps REQUIRED)
find_package(log4cxx REQUIRED)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

link_directories(${CMAKE_INSTALL_PREFIX}/lib)
link_directories(${QUICKFIX_INSTALL_PREFIX}/lib)

file(GLOB DISTRIBUTED_SE_LIB *)
include_directories(${CMAKE_INSTALL_PREFIX}/include)
add_library(DistributedSELib SHARED ${DISTRIBUTED_SE_LIB})
target_link_libraries(DistributedSELib quickfix fastcdr fastrtps log4cxx)

install(TARGETS DistributedSELib PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/dse)