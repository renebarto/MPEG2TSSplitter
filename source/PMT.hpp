#pragma once

#include <cstdint>
#include <deque>
#include <string>
#include <vector>
#include "PSI.hpp"
#include "StreamInfo.hpp"

namespace Media
{

struct ElementaryStreamInfo
{
    uint16_t programNumber;
    ProgramDescriptorList programDescriptors;
    StreamInfoList streamInfo;
};

class PMT : public PSI
{
public:
    PMT();

    bool Parse(std::deque<uint8_t> & data) override;
    bool IsValid() const override;

    bool HaveAudio() const;
    bool HaveVideo() const;

    PIDType GetAudioPID() const;
    PIDType GetVideoPID() const;

    void DisplayContents() const override;

private:
    uint16_t _programNumber;
    PIDType _pcrPID;
    uint16_t _programInfoLength;
    ElementaryStreamInfo _streamInfo;

    bool ParseDescriptor(Tools::BitBuffer & buffer, ProgramDescriptorList & list);
    bool ParseStreamInfo(Tools::BitBuffer & buffer);
};


} // namespace Media
