#pragma once

#include <cstdint>
#include <deque>
#include <vector>
#include "TSDefinitions.hpp"

namespace Media
{

enum class StreamID : uint8_t
{
    ProgramStreamMap = 0xBC,
    PrivateStream1 = 0xBD,
    PaddingStream = 0xBE,
    PrivateStream2 = 0xBF,
    ECMStream = 0xF0,
    EMMStream = 0xF1,
    DSMCCStream = 0xF2,
    IEC13522Stream = 0xF3,
    ITU_T_H222_1_A_Stream = 0xF4,
    ITU_T_H222_1_B_Stream = 0xF5,
    ITU_T_H222_1_C_Stream = 0xF6,
    ITU_T_H222_1_D_Stream = 0xF7,
    ITU_T_H222_1_E_Stream = 0xF8,
    AncillaryStream = 0xF9,
    IEC14496_1_SLPacketizedStream = 0xFA,
    IEC14496_1_FlexMuxStream = 0xFB,
    Reserved1 = 0xFC,
    Reserved2 = 0xFD,
    Reserved3 = 0xFE,
    ProgramStreamDirectory = 0xFF,
};

enum class PTSDTSFlags : uint8_t
{
    NoDTSPTS = 0,
    Forbidden = 1,
    PTSOnly = 2,
    PTSAndDTS = 3,
};
class PESPacket
{
public:
    PESPacket();

    bool Parse(std::deque<uint8_t> & data);
    bool NeedMoreData() const { return _needMoreData; }
    bool IsValid() const;
    void DisplayContents() const;
    bool IsAudioStream() const;
    bool IsVideoStream() const;
    bool IsPaddingStream() const;
    bool StreamHasHeader() const;

private:
    uint32_t _startCode;
    StreamID _streamID;
    uint16_t _packetLength;
    bool _endOfPacket;
    std::vector<uint8_t> _packetData;
    bool _needMoreData;
    ScrambingControl _PESScramblingControl;
    bool _PESPriority;
    bool _dataAlignmentIndicator;
    bool _copyright;
    bool _originalOrCopy;
    PTSDTSFlags _PTSDTSFlags;
    bool _ESCRFlags;
    bool _ESRateFlag;
    bool _DSMTrickModeFlag;
    bool _additionalCopyInfoFlag;
    bool _PES_CRCFlag;
    bool _PESExtensionFlag;
    uint8_t _PESHeaderDataLength;
    uint64_t _PTS;
    uint64_t _DTS;
};

} // namespace Media
