#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "tools/Definitions.hpp"
#include "media/TSDefinitions.hpp"

namespace Media
{

class IStreamCallback;

struct ProgramAssociation
{
    uint16_t programNumber;
    PIDType pid;
};

using ProgramAssociationList = std::vector<ProgramAssociation>;

class PAT
{
public:
    explicit PAT(IStreamCallback * streamInfoCallback);

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
    uint16_t _transportStreamID;
    ProgramAssociationList _programInfo;
};


} // namespace Media
