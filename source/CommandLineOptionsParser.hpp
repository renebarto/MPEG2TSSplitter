#pragma once

#include <string>
#include "tools/CommandLineParser.hpp"

using namespace std;

class CommandLineOptionsParser : public Tools::CommandLineParser
{
public:
    CommandLineOptionsParser(Tools::Console & console);

    string _inputPath;
    bool _verbose;
    uint16_t _audioPID;
    uint16_t _videoPID;
};


