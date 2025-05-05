cmake_minimum_required(VERSION 3.15)
project(DistributedStockExchangeLib LANGUAGES CXX)

# Find requirements
find_package(fastcdr REQUIRED)
find_package(fastdds 3 REQUIRED)
find_package(log4cxx REQUIRED)

# C++ Standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

link_directories(${CMAKE_INSTALL_PREFIX}/lib)
link_directories(${QUICKFIX_INSTALL_PREFIX}/lib)

file(GLOB DISTRIBUTED_SE_LIB *)
add_library(DistributedStockExchangeLib SHARED ${DISTRIBUTED_SE_LIB})
include_directories(${CMAKE_INSTALL_PREFIX}/include)
target_link_libraries(DistributedStockExchangeLib quickfix fastcdr fastdds log4cxx)

install(TARGETS DistributedStockExchangeLib PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/dse)