#pragma once

#include <iostream>
#include <iomanip>

namespace Media
{

enum class StreamType : uint8_t
{
    Reserved = 0x00,
    VideoIEC11172 = 0x01,
    VideoIEC13818_2 = 0x02,
    AudioIEC11172 = 0x03,
    AudioIEC13818_3 = 0x04,
    PrivateSections = 0x05,
    PESPrivateData = 0x06,
    MHEG_IEC13522 = 0x07,
    DSM_CC = 0x08,
    ITU_H222_1 = 0x09,
    IEC_13818_6A = 0x0A,
    IEC_13818_6B = 0x0B,
    IEC_13818_6C = 0x0C,
    IEC_13818_6D = 0x0D,
    AuxiliaryIEC13818_1 = 0x0E,
    AudioIEC13818_7 = 0x0F,
    VisualIEC14496_2 = 0x10,
    AudioIEC14496_3 = 0x11,
    IEC14496_1_PESPackets = 0x12,
    IEC14496_1_IEC14496Sections = 0x13,
    IEC13818_6_SynchronizedDownload = 0x14,
    MetadataPESPackets = 0x15,
    MetadataMetadataSections = 0x16,
    MetadataIEC13838_6_DataCarousel = 0x17,
    MetadataIEC13838_6_ObjectCarousel = 0x18,
    MetadataIEC13838_6_SynchronizedDownload = 0x19,
    IMPMPIEC13818_11 = 0x1A,
    VideoIEC14496_10_AVC = 0x1B,
    AudioIEC14496_3_NoTransportSyntax = 0x1C,
    TextIEC14496_17 = 0x1D,
    ReservedFirst = 0x1E,
    ReservedLast = 0x7F,
    UserFirst = 0x80,
    UserLast = 0xFF,
};

inline std::ostream & operator << (std::ostream & stream, StreamType type)
{
    switch (type)
    {
        case StreamType::Reserved:
            stream << "ITU-T | ISO/IEC Reserved"; return stream;
        case StreamType::VideoIEC11172:
            stream << "ISO/IEC 11172 Video"; return stream;
        case StreamType::AudioIEC13818_7:
            stream << "ISO/IEC 13818-7 Audio with ADTS transport syntax"; return stream;
        case StreamType::VideoIEC14496_10_AVC:
            stream << "AVC video stream as defined in ITU-T Rec. H.264 | ISO/IEC 14496-10 Video"; return stream;
        default:
            break;
    }
    stream << "UNKNOWN (" << std::hex << std::setw(2) << std::setfill('0') << (int)type << std::dec << ")";
    return stream;
}

} // namespace Media
