#pragma once

#include <cstdint>
#include <deque>
#include <string>
#include <vector>
#include "PSI.hpp"

namespace Media
{

struct ProgramAssociation
{
    uint16_t programNumber;
    PIDType pid;
};

using ProgramAssociationList = std::vector<ProgramAssociation>;

class PAT : public PSI
{
public:
    PAT();

    bool Parse(std::deque<uint8_t> & data) override;
    bool IsValid() const override;

    bool IsPMT(PIDType pid) const;
    bool IsNIT(PIDType pid) const { return pid == _pidNIT;}

    void DisplayContents() const override;

private:
    uint16_t _transportStreamID;
    ProgramAssociationList _programInfo;
    PIDType _pidNIT;
};


} // namespace Media
