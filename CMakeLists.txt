cmake_minimum_required(VERSION 3.15)
project(DistributedStockExchange LANGUAGES CXX)

# Find requirements
find_package(fastcdr REQUIRED)
find_package(fastdds 3 REQUIRED)

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

find_package(Boost 1.40 REQUIRED COMPONENTS program_options)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

link_directories(${QUICKFIX_INSTALL_PREFIX}/lib)

include_directories(library)

add_subdirectory(library/dds/idl)
include_directories(library/dds/idl)

add_subdirectory(fix-gateway/src)
# add_subdirectory(data-service/src)
add_subdirectory(matching-engine/src)

set(CMAKE_XCODE_ATTRIBUTE_OTHER_CODE_SIGN_FLAGS "-o linker-signed")

install(DIRECTORY mock-stock-exchange DESTINATION ${CMAKE_INSTALL_PREFIX})
install(CODE "
    file(GLOB_RECURSE SCRIPT_FILES
        \"\${CMAKE_INSTALL_PREFIX}/mock-stock-exchange/*.sh\")
    foreach(script \${SCRIPT_FILES})
        execute_process(COMMAND chmod +x \${script})
    endforeach()
")
install(FILES 	fix-gateway/scripts/fixgateway.sh data-service/scripts/dataservice.sh matching-engine/scripts/matchingengine.sh DESTINATION scripts)
install(FILES 	config/log4cxx.xml DESTINATION config)
install(FILES 	fix-gateway/spec/Fix44.xml DESTINATION spec)