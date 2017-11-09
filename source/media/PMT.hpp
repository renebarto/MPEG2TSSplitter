#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "media/ProgramDescriptor.hpp"
#include "media/StreamInfo.hpp"

namespace Media
{

class IStreamCallback;

struct ElementaryStreamInfo
{
    uint16_t programNumber;
    ProgramDescriptorList programDescriptors;
    StreamInfoList streamInfo;
};

class PMT
{
public:
    PMT(IStreamCallback * streamInfoCallback);

    bool Parse(ByteIterator start, ByteIterator end);
    bool IsValid() const;
    bool NeedMoreData() const { return _needMoreData; }

    void DisplayContents() const;

private:
    IStreamCallback * _streamInfoCallback;
    TableID _tableID;
    uint16_t _sectionLength;
    bool _needMoreData;
    uint8_t _versionNumber;
    bool _current;
    uint8_t _sectionNumber;
    uint8_t _lastSectionNumber;
    uint32_t _crc;
    uint16_t _programNumber;
    PIDType _pcrPID;
    uint16_t _programInfoLength;
    ElementaryStreamInfo _streamInfo;

    bool ParseDescriptor(Tools::BitBuffer & buffer, ProgramDescriptorList & list);
    bool ParseStreamInfo(Tools::BitBuffer & buffer);
};


} // namespace Media
