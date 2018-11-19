cmake_minimum_required(VERSION 3.7)

get_filename_component(DATA_DIR "${CMAKE_CURRENT_LIST_DIR}/../data" ABSOLUTE)
get_filename_component(SCRIPTS_DIR "${CMAKE_CURRENT_LIST_DIR}/../scripts" ABSOLUTE)
set(CMAKE_MODULE_PATH "${SCRIPTS_DIR}/cmake/" ${CMAKE_MODULE_PATH} )

include("${SCRIPTS_DIR}/cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.16.15.tar.gz"
    SHA1 "6974c2150fc0d3b09de3ad1efcbf15d360647ffa"
)


project(csvcomparator)


#====================================================================
# Set global definitions
#====================================================================
add_definitions(-std=c++14)
add_definitions(-Wall)
add_definitions(-Werror)
add_definitions(-DPROJECT_DIR="${PROJECT_SOURCE_DIR}")
add_definitions(-DDATA_DIR="${DATA_DIR}")


#====================================================================
# find required external libraries
#====================================================================
hunter_add_package(GTest)
#hunter_add_package(Boost COMPONENTS system filesystem)

find_package(Threads)
find_package(Boost REQUIRED COMPONENTS system filesystem)  # more info with command: cmake --help-module FindBoost


#====================================================================
# Add subdirectories
#====================================================================
enable_testing()
add_subdirectory(lib)
add_subdirectory(test)
add_subdirectory(src)
