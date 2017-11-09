project(mpeg2ts-splitter-test)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

unset(GTEST_HAS_PTHREAD)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads)
if (CMAKE_USE_PTHREADS_INIT)
    set(GTEST_HAS_PTHREAD ON)
endif()

include(setup_target_properties_executable)
include(show_target_properties)
include(display_list)

message("Setting up unit test")

set(GOOGLE_TEST_BASE ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/)
add_library(gtest STATIC ${GOOGLE_TEST_BASE}/src/gtest-all.cc)
target_include_directories(gtest PUBLIC ${GOOGLE_TEST_BASE}/include ${GOOGLE_TEST_BASE})
add_library(gtest_main STATIC ${GOOGLE_TEST_BASE}/src/gtest_main.cc)
target_include_directories(gtest_main PUBLIC ${GOOGLE_TEST_BASE}/include ${GOOGLE_TEST_BASE})
target_link_libraries(gtest_main gtest)

set(UNIT_TEST_DEFINITIONS
    ${COMPILER_DEFINITIONS_CXX}
    TEST_DATA_ROOT="${TESTDATAROOT}"
    UNIT_TEST)

set(UNIT_TEST_INCLUDE_DIRS
    ../source
    ${GOOGLE_TEST_BASE}/include)

set(UNIT_TEST_OPTIONS
    ${COMPILER_OPTIONS_CXX})

set(UNIT_TEST_LINK_OPTIONS)

set(UNIT_TEST_DEPENDENCIES
    Media
    Tools
    gtest_main)

set(UNIT_TEST_LIBS
    ${CMAKE_THREAD_LIBS_INIT}
    ${UNIT_TEST_DEPENDENCIES})

file(GLOB UNIT_TEST_INCLUDES *.h)

set(UNIT_TEST_SOURCES
    main.cpp
    BitBufferTest.cpp
    )

set(UNIT_TEST_INPUT
    ${UNIT_TEST_SOURCES}
    ${UNIT_TEST_INCLUDES}
    MPEG2SplitterTest.cpp)

display_list("Defines                     : " ${UNIT_TEST_DEFINITIONS} )
display_list("Compiler options            : " ${UNIT_TEST_OPTIONS} )
display_list("Source files                : " ${UNIT_TEST_SOURCES} )
display_list("Include files               : " ${UNIT_TEST_INCLUDES} )
display_list("Include dirs                : " ${UNIT_TEST_INCLUDE_DIRS} )
display_list("Link libs                   : " ${UNIT_TEST_LIBS} )
display_list("Linker options              : " ${UNIT_TEST_LINK_OPTIONS} )
display_list("Dependencies                : " ${UNIT_TEST_DEPENDENCIES} )

add_executable(${PROJECT_NAME} ${UNIT_TEST_INPUT})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${UNIT_TEST_DEFINITIONS})
target_include_directories(${PROJECT_NAME} PRIVATE ${UNIT_TEST_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${UNIT_TEST_LIBS})
list_to_string(UNIT_TEST_LINK_OPTIONS UNIT_TEST_LINK_OPTIONS_STRING)
if (NOT "${UNIT_TEST_LINK_OPTIONS_STRING}" STREQUAL "")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${UNIT_TEST_LINK_OPTIONS_STRING}")
endif()
setup_target_properties_executable(${PROJECT_NAME})

show_target_properties(${PROJECT_NAME})

