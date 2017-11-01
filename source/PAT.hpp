#pragma once

#include <cstdint>
#include <deque>
#include <string>
#include "TSDefinitions.hpp"

namespace Media
{

class PAT
{
public:
    PAT();

    bool Parse(std::deque<uint8_t> & buffer);
    bool IsValid() const;

    uint16_t GetPID_PMT() const { return _pidPMT;}
    uint16_t GetPID_NIT() const { return _pidNIT;}

    std::string ToString() const;

private:
    TableID _tableID;
    uint16_t _sectionLength;
    uint16_t _pidPMT;
    uint16_t _pidNIT;
};


} // namespace Media
