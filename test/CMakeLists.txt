set(tests
    serialport_test
    serialevent_test
)
include_directories(
    ${PROJECT_SOURCE_DIR}/src
)
foreach(test IN LISTS tests)
    add_executable(${test} ${test}.c)
    target_link_libraries(${test} ${PROJECT_NAME}_static)
endforeach()
