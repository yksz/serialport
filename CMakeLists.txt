cmake_minimum_required(VERSION 2.8.7)
project(serialport C)

set(CMAKE_C_FLAGS "-Wall -std=c89")
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_C_FLAGS_DEBUG "-g")

option(build_tests "Build all of own tests" OFF)
option(build_examples "Build example programs" OFF)

### Library
if(WIN32)
    set(source_files ${source_files}
        ${PROJECT_SOURCE_DIR}/src/serialport_windows.c
        ${PROJECT_SOURCE_DIR}/src/serialevent_windows.c
    )
else() # UNIX
    set(source_files ${source_files}
        ${PROJECT_SOURCE_DIR}/src/serialport_unix.c
        ${PROJECT_SOURCE_DIR}/src/serialevent_unix.c
    )
endif()
include_directories(
    ${PROJECT_SOURCE_DIR}/src
)
# shared and static libraries
add_library(${PROJECT_NAME} SHARED ${source_files})
add_library(${PROJECT_NAME}_static STATIC ${source_files})
set_target_properties(${PROJECT_NAME}_static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

### Install
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
file(GLOB header_files src/*.h)
install(FILES ${header_files} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

### Test
if(build_tests)
    enable_testing()
    add_subdirectory(test)
endif()

### Example
if(build_examples)
    add_subdirectory(example)
endif()
