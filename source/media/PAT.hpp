#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "media/PSI.hpp"

namespace Media
{

class IStreamCallback;

struct ProgramAssociation
{
    uint16_t programNumber;
    PIDType pid;
};

using ProgramAssociationList = std::vector<ProgramAssociation>;

class PAT : public PSI
{
public:
    explicit PAT(IStreamCallback * streamInfoCallback);

    bool Parse(std::vector<uint8_t> & data) override;
    bool IsValid() const override;

    void DisplayContents() const override;

private:
    IStreamCallback * _streamInfoCallback;
    uint16_t _transportStreamID;
    ProgramAssociationList _programInfo;
};


} // namespace Media
