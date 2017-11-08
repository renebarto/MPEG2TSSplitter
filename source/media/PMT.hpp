#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "media/PSI.hpp"
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

class PMT : public PSI
{
public:
    PMT(IStreamCallback * streamInfoCallback);

    bool Parse(std::vector<uint8_t> & data) override;
    bool IsValid() const override;

    bool HaveAudio() const;
    bool HaveVideo() const;

    PIDType GetAudioPID() const;
    PIDType GetVideoPID() const;

    void DisplayContents() const override;

private:
    IStreamCallback * _streamInfoCallback;
    uint16_t _programNumber;
    PIDType _pcrPID;
    uint16_t _programInfoLength;
    ElementaryStreamInfo _streamInfo;

    bool ParseDescriptor(Tools::BitBuffer & buffer, ProgramDescriptorList & list);
    bool ParseStreamInfo(Tools::BitBuffer & buffer);
};


} // namespace Media
