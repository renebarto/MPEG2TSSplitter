cmake_minimum_required(VERSION 3.8)
project(MPEG2TS_demux)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    main.cpp
    MPEG2Demultiplexor.cpp
    TransportStream.cpp
    TSPacket.cpp
    PEStream.cpp
    MPEG2Demultiplexor.hpp
    TransportStream.hpp
    TSPacket.hpp
    PEStream.hpp PESPacket.cpp PESPacket.hpp)

add_executable(MPEG2TS_demux ${SOURCE_FILES})