project(MPEG2TS-splitter)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(setup_target_properties_executable)
include(show_target_properties)
include(display_list)

message("Setting up ${PROJECT_NAME}")

set(LIB_NAME MPEG2SplitterLib)

set(MPEG2TS_SPLITTER_DEFINITIONS
    ${COMPILER_DEFINITIONS_CXX}
    MPEG2TS_SPLITTER)

set(MPEG2TS_SPLITTER_INCLUDE_DIRS
    )

set(MPEG2TS_SPLITTER_OPTIONS
    ${COMPILER_OPTIONS_CXX})

set(MPEG2TS_SPLITTER_LINK_OPTIONS)

set(MPEG2TS_SPLITTER_DEPENDENCIES
    )

set(MPEG2TS_SPLITTER_LIBS
    ${LIB_NAME}
    ${MPEG2TS_SPLITTER_DEPENDENCIES})

file(GLOB MPEG2TS_SPLITTER_INCLUDES *.h)

set(MPEG2TS_SPLITTER_SOURCES
    MPEG2Demultiplexor.cpp
    TransportStream.cpp
    TSPacket.cpp
    PEStream.cpp
    PESPacket.cpp
    PAT.cpp
    BitBuffer.cpp
    )

set(MPEG2TS_SPLITTER_INPUT
    ${MPEG2TS_SPLITTER_SOURCES}
    ${MPEG2TS_SPLITTER_INCLUDES}
    )

display_list("Defines                     : " ${MPEG2TS_SPLITTER_DEFINITIONS} )
display_list("Compiler options            : " ${MPEG2TS_SPLITTER_OPTIONS} )
display_list("Source files                : " ${MPEG2TS_SPLITTER_SOURCES} )
display_list("Include files               : " ${MPEG2TS_SPLITTER_INCLUDES} )
display_list("Include dirs                : " ${MPEG2TS_SPLITTER_INCLUDE_DIRS} )
display_list("Link libs                   : " ${MPEG2TS_SPLITTER_LIBS} )
display_list("Linker options              : " ${MPEG2TS_SPLITTER_LINK_OPTIONS} )
display_list("Dependencies                : " ${MPEG2TS_SPLITTER_DEPENDENCIES} )

add_library(${LIB_NAME} STATIC ${MPEG2TS_SPLITTER_INPUT})
target_compile_definitions(${LIB_NAME} PRIVATE ${MPEG2TS_SPLITTER_DEFINITIONS})
target_include_directories(${LIB_NAME} PRIVATE ${MPEG2TS_SPLITTER_INCLUDE_DIRS})

show_target_properties(${LIB_NAME})

add_executable(${PROJECT_NAME} main.cpp)
target_compile_definitions(${PROJECT_NAME} PRIVATE ${MPEG2TS_SPLITTER_DEFINITIONS})
target_include_directories(${PROJECT_NAME} PRIVATE ${MPEG2TS_SPLITTER_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${MPEG2TS_SPLITTER_LIBS})
list_to_string(MPEG2TS_SPLITTER_LINK_OPTIONS MPEG2TS_SPLITTER_LINK_OPTIONS_STRING)
if (NOT "${MPEG2TS_SPLITTER_LINK_OPTIONS_STRING}" STREQUAL "")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${MPEG2TS_SPLITTER_LINK_OPTIONS_STRING}")
endif()
setup_target_properties_executable(${PROJECT_NAME})

show_target_properties(${PROJECT_NAME})
