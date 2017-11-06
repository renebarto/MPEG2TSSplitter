#include "tools/CommandLineOptionGroup.hpp"

using namespace std;
using namespace Tools;

CommandLineOptionGroup::CommandLineOptionGroup(const std::string & name, const std::string description /*= ""*/)
    : _name(name)
    , _description(description)
    , _options()
{
}

CommandLineOptionGroup::~CommandLineOptionGroup()
{
    _options.clear();
}

void CommandLineOptionGroup::AddOption(const CommandLineOption::Ptr option)
{
    _options.push_back(option);
}
