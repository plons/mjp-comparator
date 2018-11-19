file(GLOB_RECURSE libSources *.cpp)
add_library(${PROJECT_NAME} STATIC
    ${libSources}
)
target_include_directories(${PROJECT_NAME} INTERFACE
    .
)
target_link_libraries(${PROJECT_NAME} PUBLIC
    Boost::boost
    Boost::filesystem
    Boost::system
)