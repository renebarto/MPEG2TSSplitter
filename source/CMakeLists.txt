project(MPEG2TS-splitter)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(setup_target_properties_executable)
include(show_target_properties)
include(display_list)

set(TOOLS_LIB Tools)
message("Setting up ${TOOLS_LIB}")

set(TOOLS_LIB_DEFINITIONS
    ${COMPILER_DEFINITIONS_CXX}
    TOOLS_LIB)

set(TOOLS_LIB_INCLUDE_DIRS
    .)

set(TOOLS_LIB_OPTIONS
    ${COMPILER_OPTIONS_CXX})

set(TOOLS_LIB_LINK_OPTIONS)

set(TOOLS_LIB_DEPENDENCIES
    )

set(TOOLS_LIB_LIBS
    ${TOOLS_LIB_DEPENDENCIES})

file(GLOB TOOLS_LIB_INCLUDES tools/*.hpp)

set(TOOLS_LIB_SOURCES
    tools/BitBuffer.cpp
    tools/Util.cpp
    tools/CommandLineParser.cpp
    tools/CommandLineOption.cpp
    tools/CommandLineOptionGroup.cpp
    tools/Console.cpp
    tools/DeserializationImpl.cpp
    tools/SerializationImpl.cpp
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

add_library(${TOOLS_LIB} STATIC ${TOOLS_LIB_INPUT})
target_compile_definitions(${TOOLS_LIB} PRIVATE ${TOOLS_LIB_DEFINITIONS})
target_include_directories(${TOOLS_LIB} PRIVATE ${TOOLS_LIB_INCLUDE_DIRS})

show_target_properties(${TOOLS_LIB})

set(MEDIA_LIB Media)
message("Setting up ${MEDIA_LIB}")

set(MEDIA_LIB_DEFINITIONS
    ${COMPILER_DEFINITIONS_CXX}
    MEDIA_LIB)

set(MEDIA_LIB_INCLUDE_DIRS
    .)

set(MEDIA_LIB_OPTIONS
    ${COMPILER_OPTIONS_CXX})

set(MEDIA_LIB_LINK_OPTIONS)

set(MEDIA_LIB_DEPENDENCIES
    ${TOOLS_LIB})

set(MEDIA_LIB_LIBS
    ${MEDIA_LIB_DEPENDENCIES})

file(GLOB MEDIA_LIB_INCLUDES *.hpp)

set(MEDIA_LIB_SOURCES
    MPEG2Splitter.cpp
    TransportStream.cpp
    TSPacket.cpp
    PEStream.cpp
    PESPacket.cpp
    PAT.cpp
    PMT.cpp
    ProgramDescriptor.cpp
    StreamInfo.cpp
    )

set(MEDIA_LIB_INPUT
    ${MEDIA_LIB_SOURCES}
    ${MEDIA_LIB_INCLUDES})

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

message("Setting up ${PROJECT_NAME}")

set(MPEG2TS_SPLITTER_DEFINITIONS
    ${COMPILER_DEFINITIONS_CXX}
    MPEG2TS_SPLITTER)

set(MPEG2TS_SPLITTER_INCLUDE_DIRS
    .)

set(MPEG2TS_SPLITTER_OPTIONS
    ${COMPILER_OPTIONS_CXX})

set(MPEG2TS_SPLITTER_LINK_OPTIONS)

set(MPEG2TS_SPLITTER_DEPENDENCIES
    ${MEDIA_LIB}
    ${TOOLS_LIB}
    )

set(MPEG2TS_SPLITTER_LIBS
    ${MPEG2TS_SPLITTER_DEPENDENCIES})

file(GLOB MPEG2TS_SPLITTER_INCLUDES *.hpp)

set(MPEG2TS_SPLITTER_SOURCES
    main.cpp
    CommandLineOptionsParser.cpp
    )

set(MPEG2TS_SPLITTER_INPUT
    ${MPEG2TS_SPLITTER_SOURCES}
    ${MPEG2TS_SPLITTER_INCLUDES})

display_list("Defines                     : " ${MPEG2TS_SPLITTER_DEFINITIONS} )
display_list("Compiler options            : " ${MPEG2TS_SPLITTER_OPTIONS} )
display_list("Source files                : " ${MPEG2TS_SPLITTER_SOURCES} )
display_list("Include files               : " ${MPEG2TS_SPLITTER_INCLUDES} )
display_list("Include dirs                : " ${MPEG2TS_SPLITTER_INCLUDE_DIRS} )
display_list("Link libs                   : " ${MPEG2TS_SPLITTER_LIBS} )
display_list("Linker options              : " ${MPEG2TS_SPLITTER_LINK_OPTIONS} )
display_list("Dependencies                : " ${MPEG2TS_SPLITTER_DEPENDENCIES} )

add_executable(${PROJECT_NAME} ${MPEG2TS_SPLITTER_INPUT})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${MPEG2TS_SPLITTER_DEFINITIONS})
target_include_directories(${PROJECT_NAME} PRIVATE ${MPEG2TS_SPLITTER_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${MPEG2TS_SPLITTER_LIBS})
list_to_string(MPEG2TS_SPLITTER_LINK_OPTIONS MPEG2TS_SPLITTER_LINK_OPTIONS_STRING)
if (NOT "${MPEG2TS_SPLITTER_LINK_OPTIONS_STRING}" STREQUAL "")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${MPEG2TS_SPLITTER_LINK_OPTIONS_STRING}")
endif()
setup_target_properties_executable(${PROJECT_NAME})

show_target_properties(${PROJECT_NAME})
