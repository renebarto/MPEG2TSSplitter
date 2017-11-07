project(Tools)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(show_target_properties)
include(display_list)

set(TOOLS_LIB ${PROJECT_NAME})
message("Setting up ${TOOLS_LIB}")

set(TOOLS_LIB_DEFINITIONS
    ${COMPILER_DEFINITIONS_CXX}
    TOOLS_LIB)

set(TOOLS_LIB_INCLUDE_DIRS
    ..)

set(TOOLS_LIB_OPTIONS
    ${COMPILER_OPTIONS_CXX})

set(TOOLS_LIB_LINK_OPTIONS)

set(TOOLS_LIB_DEPENDENCIES
    )

set(TOOLS_LIB_LIBS
    ${TOOLS_LIB_DEPENDENCIES})

file(GLOB TOOLS_LIB_INCLUDES *.hpp)

set(TOOLS_LIB_SOURCES
    BitBuffer.cpp
    Util.cpp
    CommandLineParser.cpp
    CommandLineOption.cpp
    CommandLineOptionGroup.cpp
    Console.cpp
    DeserializationImpl.cpp
    SerializationImpl.cpp
    Stopwatch.cpp
    )

set(TOOLS_LIB_INPUT
    ${TOOLS_LIB_SOURCES}
    ${TOOLS_LIB_INCLUDES})

display_list("Defines                     : " ${TOOLS_LIB_DEFINITIONS} )
display_list("Compiler options            : " ${TOOLS_LIB_OPTIONS} )
display_list("Source files                : " ${TOOLS_LIB_SOURCES} )
display_list("Include files               : " ${TOOLS_LIB_INCLUDES} )
display_list("Include dirs                : " ${TOOLS_LIB_INCLUDE_DIRS} )
display_list("Link libs                   : " ${TOOLS_LIB_LIBS} )
display_list("Linker options              : " ${TOOLS_LIB_LINK_OPTIONS} )
display_list("Dependencies                : " ${TOOLS_LIB_DEPENDENCIES} )

add_library(${PROJECT_NAME} STATIC ${TOOLS_LIB_INPUT})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${TOOLS_LIB_DEFINITIONS})
target_include_directories(${PROJECT_NAME} PRIVATE ${TOOLS_LIB_INCLUDE_DIRS})

show_target_properties(${PROJECT_NAME})

