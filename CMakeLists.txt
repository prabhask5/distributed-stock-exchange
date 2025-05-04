cmake_minimum_required(VERSION 3.15)
project(DistributedStockExchange LANGUAGES CXX)

# Find requirements
find_package(fastcdr REQUIRED)
find_package(fastdds REQUIRED)

# C++ Standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost 1.40 REQUIRED COMPONENTS program_options)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

link_directories(${QUICKFIX_INSTALL_PREFIX}/lib)

include_directories(lib)

# add_subdirectory(lib/idl)
# include_directories(lib/idl)

# add_subdirectory(fix-gateway/src)
# add_subdirectory(data-service/src)
add_subdirectory(matching-engine/src)

set(CMAKE_XCODE_ATTRIBUTE_OTHER_CODE_SIGN_FLAGS "-o linker-signed")

install(DIRECTORY mock-stock-exchange DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES 	fix-gateway/scripts/fixgateway.sh data-service/scripts/dataservice.sh matching-engine/scripts/matchingengine.sh DESTINATION scripts)
install(FILES 	config/log4cxx.xml DESTINATION config)
install(FILES 	fix-gateway/spec/Fix44.xml DESTINATION spec)