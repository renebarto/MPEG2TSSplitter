project(Media)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(show_target_properties)
include(display_list)

set(MEDIA_LIB ${PROJECT_NAME})
message("Setting up ${MEDIA_LIB}")

set(MEDIA_LIB_DEFINITIONS
    ${COMPILER_DEFINITIONS_CXX}
    MEDIA_LIB)

set(MEDIA_LIB_INCLUDE_DIRS
    ..)

set(MEDIA_LIB_OPTIONS
    ${COMPILER_OPTIONS_CXX})

set(MEDIA_LIB_LINK_OPTIONS)

set(MEDIA_LIB_DEPENDENCIES
    Tools)

set(MEDIA_LIB_LIBS
    ${MEDIA_LIB_DEPENDENCIES})

file(GLOB MEDIA_LIB_INCLUDES *.hpp)

set(MEDIA_LIB_SOURCES
    MPEG2Splitter.cpp
    TransportStream.cpp
    TSPacket.cpp
    PESPacket.cpp
    PAT.cpp
    PMT.cpp
    ProgramDescriptor.cpp
    StreamInfo.cpp
    StreamWriter.cpp
    PATHandler.cpp
    PMTHandler.cpp
    AudioStreamHandler.cpp
    VideoStreamHandler.cpp
    )

set(MEDIA_LIB_INPUT
    ${MEDIA_LIB_SOURCES}
    ${MEDIA_LIB_INCLUDES} Logging.hpp Logging.cpp)

display_list("Defines                     : " ${MEDIA_LIB_DEFINITIONS} )
display_list("Compiler options            : " ${MEDIA_LIB_OPTIONS} )
display_list("Source files                : " ${MEDIA_LIB_SOURCES} )
display_list("Include files               : " ${MEDIA_LIB_INCLUDES} )
display_list("Include dirs                : " ${MEDIA_LIB_INCLUDE_DIRS} )
display_list("Link libs                   : " ${MEDIA_LIB_LIBS} )
display_list("Linker options              : " ${MEDIA_LIB_LINK_OPTIONS} )
display_list("Dependencies                : " ${MEDIA_LIB_DEPENDENCIES} )

add_library(${MEDIA_LIB} STATIC ${MEDIA_LIB_INPUT})
target_compile_definitions(${MEDIA_LIB} PRIVATE ${MEDIA_LIB_DEFINITIONS})
target_include_directories(${MEDIA_LIB} PRIVATE ${MEDIA_LIB_INCLUDE_DIRS})

show_target_properties(${MEDIA_LIB})

