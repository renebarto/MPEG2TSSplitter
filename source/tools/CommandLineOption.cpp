#include "tools/CommandLineOption.hpp"

using namespace Tools;

template<>
std::vector<EnumConversion<CommandLineArgumentType>> EnumSerializationInfo<CommandLineArgumentType>::Info =
{
    {CommandLineArgumentType::NoArgument,       "None"},
    {CommandLineArgumentType::OptionalArgument, "Optional"},
    {CommandLineArgumentType::RequiredArgument, "Required"},
};

CommandLineOption::CommandLineOption(const std::string & longName, char shortName,
                                     const std::string & description,
                                     CommandLineArgumentType argType)
    : _longName(longName)
    , _shortName(shortName)
    , _description(description)
    , _argType(argType)
    , _textValue(nullptr)
    , _optionFound()
    , _argument()
{
}

CommandLineOption::CommandLineOption(const std::string & longName, char shortName,
                                     const std::string & description,
                                     std::string & textVariable,
                                     CommandLineArgumentType argType)
    : _longName(longName)
    , _shortName(shortName)
    , _description(description)
    , _argType(argType)
    , _textValue(&textVariable)
    , _optionFound()
    , _argument()
{

}

