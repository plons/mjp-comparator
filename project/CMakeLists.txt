cmake_minimum_required(VERSION 2.8)
project(csvcomparator)

add_definitions(-std=c++11)
add_definitions(-Wall)
add_definitions(-Werror)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
message(STATUS "Including: ${GTEST_INCLUDE_DIRS}")
message(STATUS "Libs: ${GTEST_BOTH_LIBRARIES}")

find_package(Threads)

#====================================================================
# LINK BOOST LIBRARY
#     more info with command: cmake --help-module FindBoost
#     or on http://www.vtk.org/Wiki/CMake:How_To_Find_Libraries
#====================================================================
set(Boost_USE_STATIC_LIBS       OFF)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost COMPONENTS thread system filesystem date_time regex program_options iostreams chrono timer log REQUIRED)
message(STATUS "Found Boost_INCLUDE_DIRS:            ${Boost_INCLUDE_DIRS}") 
message(STATUS "Found Boost_FILESYSTEM_LIBRARY:      ${Boost_FILESYSTEM_LIBRARY}")
message(STATUS "Found Boost_SYSTEM_LIBRARY:          ${Boost_SYSTEM_LIBRARY}")
message(STATUS "Found Boost_DATE_TIME_LIBRARY:       ${Boost_DATE_TIME_LIBRARY}")
message(STATUS "Found Boost_REGEX_LIBRARY:           ${Boost_REGEX_LIBRARY}")
message(STATUS "Found Boost_PROGRAM_OPTIONS:         ${Boost_PROGRAM_OPTIONS_LIBRARY}")
message(STATUS "Found Boost_IOSTREAMS_LIBRARY:       ${Boost_IOSTREAMS_LIBRARY}")
message(STATUS "Found Boost_CHRONO_LIBRARY:          ${Boost_CHRONO_LIBRARY}")
message(STATUS "Found Boost_TIMER_LIBRARY:           ${Boost_TIMER_LIBRARY}")
message(STATUS "Found Boost_LOG_LIBRARY:             ${Boost_LOG_LIBRARY}")
include_directories(${Boost_INCLUDE_DIRS})            # => header only libraries are ok


# build library
include_directories(lib)
file(GLOB_RECURSE libSources lib/*.cpp)
if (libSources)
    message(STATUS "Adding library lib${PROJECT_NAME}")
    add_library(${PROJECT_NAME} STATIC ${libSources})
endif()


# build tests
enable_testing()
include_directories(test)
file(GLOB testSources test/*.cpp)
message(STATUS "Adding test executable test_${PROJECT_NAME}")
add_executable(test_${PROJECT_NAME} ${testSources})
if (libSources)
    target_link_libraries(test_${PROJECT_NAME} ${PROJECT_NAME})
endif()
target_link_libraries(test_${PROJECT_NAME}
    ${GTEST_BOTH_LIBRARIES}
    /usr/lib/libgmock.a
    /usr/lib/libgmock_main.a
    ${Boost_REGEX_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
)

# Copy the executable so it doesn't get removed if the tests fail
add_custom_command(TARGET test_${PROJECT_NAME} POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy 
        ${CMAKE_CURRENT_BINARY_DIR}/test_${PROJECT_NAME}
        ${CMAKE_CURRENT_BINARY_DIR}/test_${PROJECT_NAME}_exe
)

# Execute the tests after building the library and tests
add_custom_command(TARGET test_${PROJECT_NAME} POST_BUILD 
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test_${PROJECT_NAME}
)
add_test(NAME test_${PROJECT_NAME} COMMAND test_${PROJECT_NAME})

