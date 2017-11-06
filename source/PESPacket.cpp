#include "PESPacket.hpp"

#include <iostream>
#include <iomanip>
#include <tools/SerializationImpl.hpp>
#include "tools/BitBuffer.hpp"
#include "tools/Console.hpp"

using namespace std;
using namespace Media;

constexpr uint32_t StartCodeValue = 0x000001;
constexpr uint8_t AudioStreamMask = 0xE0;
constexpr uint8_t AudioStreamID = 0xC0;
constexpr uint8_t VideoStreamMask = 0xF0;
constexpr uint8_t VideoStreamID = 0xE0;

PESPacket::PESPacket()
    : _startCode()
    , _streamID()
    , _packetLength()
    , _endOfPacket()
    , _packetData()
    , _needMoreData(true)
    , _PESScramblingControl()
    , _PESPriority()
    , _dataAlignmentIndicator()
    , _copyright()
    , _originalOrCopy()
    , _PTSDTSFlags()
    , _ESCRFlags()
    , _ESRateFlag()
    , _DSMTrickModeFlag()
    , _additionalCopyInfoFlag()
    , _PES_CRCFlag()
    , _PESExtensionFlag()
    , _PESHeaderDataLength()
{
}

bool PESPacket::Parse(std::deque<uint8_t> & data)
{
    Tools::BitBuffer buffer;
    buffer.SetData(data);
    _startCode = static_cast<uint32_t>(buffer.ReadBits(24));
    if (_startCode != StartCodeValue)
    {
        return false;
    }
    _streamID = static_cast<StreamID >(buffer.ReadBits(8));
    _packetLength = static_cast<uint16_t>(buffer.ReadBits(16));
    if (StreamHasHeader())
    {
        if (buffer.ReadBits(2) != 0x00000002)  // Must be 10
            return false;
        _PESScramblingControl = static_cast<ScrambingControl>(buffer.ReadBits(2));
        _PESPriority = buffer.ReadBit();
        _dataAlignmentIndicator = buffer.ReadBit();
        _copyright = buffer.ReadBit();
        _originalOrCopy = buffer.ReadBit();
        _PTSDTSFlags = static_cast<PTSDTSFlags>(buffer.ReadBits(2));
        _ESCRFlags = buffer.ReadBit();
        _ESRateFlag = buffer.ReadBit();
        _DSMTrickModeFlag = buffer.ReadBit();
        _additionalCopyInfoFlag = buffer.ReadBit();
        _PES_CRCFlag = buffer.ReadBit();
        _PESExtensionFlag = buffer.ReadBit();
        _PESHeaderDataLength = static_cast<uint8_t>(buffer.ReadBits(8));
        if ((_PTSDTSFlags == PTSDTSFlags::PTSOnly) ||
            (_PTSDTSFlags == PTSDTSFlags::PTSAndDTS))
        {
            if (static_cast<uint8_t>(buffer.ReadBits(4)) != static_cast<uint8_t>(_PTSDTSFlags))  // Must be equal to flags
                return false;
            _PTS = buffer.ReadBits(3) << 30;
            if (!buffer.ReadBit())                  // Marker bit = 1
                return false;
            _PTS |= buffer.ReadBits(15) << 15;
            if (!buffer.ReadBit())                  // Marker bit = 1
                return false;
            _PTS |= buffer.ReadBits(15) << 0;
            if (!buffer.ReadBit())                  // Marker bit = 1
                return false;
            if (_PTSDTSFlags == PTSDTSFlags::PTSAndDTS)
            {
                if (static_cast<uint8_t>(buffer.ReadBits(4)) != 0x01)  // Must be 0001
                    return false;
                _DTS = buffer.ReadBits(3) << 30;
                if (!buffer.ReadBit())                  // Marker bit = 1
                    return false;
                _DTS |= buffer.ReadBits(15) << 15;
                if (!buffer.ReadBit())                  // Marker bit = 1
                    return false;
                _DTS |= buffer.ReadBits(15) << 0;
                if (!buffer.ReadBit())                  // Marker bit = 1
                    return false;
            }
        }
        if (_ESCRFlags)
        {
            throw std::logic_error("Not implemented");
        }
        if (_ESRateFlag)
        {
            throw std::logic_error("Not implemented");
        }
        if (_DSMTrickModeFlag)
        {
            throw std::logic_error("Not implemented");
        }
        if (_additionalCopyInfoFlag)
        {
            throw std::logic_error("Not implemented");
        }
        if (_PES_CRCFlag)
        {
            throw std::logic_error("Not implemented");
        }
        if (_PESExtensionFlag)
        {
            throw std::logic_error("Not implemented");
        }
        while (static_cast<uint8_t>(buffer.ReadAheadBits(8)) == 0xFF)
            buffer.ReadBits(8);
    }

    _needMoreData = (buffer.BytesLeft() < _packetLength) || ((_packetLength == 0) && !_endOfPacket);
    if (_needMoreData)
        return false;
    if (IsPaddingStream())
    {
        for (size_t i = 0; i < _packetLength; ++i)
            buffer.ReadBits(8); // Simple consume padding bytes
    }
    else
    {
        for (size_t i = 0; i < _packetLength; ++i)
            _packetData.push_back(static_cast<uint8_t>(buffer.ReadBits(8)));
    }
    return IsValid();
}

bool PESPacket::IsValid() const
{
    return (_startCode == StartCodeValue) &&
        (_PTSDTSFlags != PTSDTSFlags::Forbidden);
}

bool PESPacket::IsAudioStream() const
{
    return (static_cast<uint8_t>(_streamID) & AudioStreamMask) == AudioStreamID;
}

bool PESPacket::IsVideoStream() const
{
    return (static_cast<uint8_t>(_streamID) & VideoStreamMask) == VideoStreamID;
}

bool PESPacket::IsPaddingStream() const
{
    return _streamID == StreamID::PaddingStream;
}

bool PESPacket::StreamHasHeader() const
{
    return (_streamID != StreamID::ProgramStreamMap) &&
           (_streamID != StreamID::PaddingStream) &&
           (_streamID != StreamID::PrivateStream2) &&
           (_streamID != StreamID::ECMStream) &&
           (_streamID != StreamID::EMMStream) &&
           (_streamID != StreamID::ProgramStreamDirectory) &&
           (_streamID != StreamID::DSMCCStream) &&
           (_streamID != StreamID::ITU_T_H222_1_E_Stream);
}

void PESPacket::DisplayContents() const
{
    Tools::DefaultConsole() << endl << "PMT" << endl << endl;
    Tools::DefaultConsole() << "StartCode:        " << Tools::Serialize(_startCode, 16) << endl;
    Tools::DefaultConsole() << "Stream ID:        " << Tools::Serialize(static_cast<uint8_t>(_streamID), 16) << endl;
    Tools::DefaultConsole() << "Audio stream:     " << (IsAudioStream() ? "Yes" : "No") << endl;
    Tools::DefaultConsole() << "Video stream:     " << (IsVideoStream() ? "Yes" : "No") << endl;
    Tools::DefaultConsole() << "Packet length:    " << Tools::Serialize(_packetLength, 10) << endl;
}
