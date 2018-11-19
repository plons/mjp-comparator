
find_package(GTest CONFIG REQUIRED)
find_package(GMock CONFIG REQUIRED)

file(GLOB testSources *.cpp)

add_executable(test_${PROJECT_NAME}
    ${testSources}
)

target_link_libraries(test_${PROJECT_NAME}
    ${PROJECT_NAME}
    GTest::gtest
    GMock::gmock
    ${EXTERNAL_LIBRARIES}
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
