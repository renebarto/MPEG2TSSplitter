#include "tools/CommandLineParser.hpp"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include "tools/Util.hpp"

using namespace std;
using namespace Tools;

const string CommandLineParser::DefaultMainOptionsGroupName = "Main";
const string CommandLineParser::HelpOptionsGroupName = "Help Options";

CommandLineParser::CommandLineParser(Console & console,
                                     const string & name,
                                     const string & description)
    : _groups()
    , _mainGroup()
    , _console(console)
    , _autoHelp(true)
    , _showHelp(false)
    , _name(name)
    , _description(description)
    , _nonOptions()
    , _options()
    , _getOptData()
{
    SetupStandardGroups();
}

CommandLineParser::~CommandLineParser()
{
    _nonOptions.clear();
    _groups.clear();
}

bool CommandLineParser::HaveOption(char shortName) const
{
    size_t index = MatchShortOption(shortName);
    return (index < numeric_limits<size_t>::max());
}

bool CommandLineParser::HaveOption(string longName) const
{
    size_t index = MatchLongOption(longName.c_str(), longName.length());
    return (index < numeric_limits<size_t>::max());
}

bool CommandLineParser::FoundOption(char shortName) const
{
    size_t index = MatchShortOption(shortName);
    return (index < numeric_limits<size_t>::max()) && _options[index]->FoundOption();
}

bool CommandLineParser::FoundOption(string longName) const
{
    size_t index = MatchLongOption(longName.c_str(), longName.length());
    return (index < numeric_limits<size_t>::max()) && _options[index]->FoundOption();
}

string CommandLineParser::GetOption(char shortName) const
{
    size_t index = MatchShortOption(shortName);
    return (index < numeric_limits<size_t>::max())
           ? _options[index]->Argument()
           : string();
}

string CommandLineParser::GetOption(string longName) const
{
    size_t index = MatchLongOption(longName.c_str(), longName.length());
    return (index < numeric_limits<size_t>::max())
           ? _options[index]->Argument()
           : string();
}

bool CommandLineParser::Parse(int argc, const char * argv[])
{
    try
    {
        bool result = InternalParse(argc, argv);
        if (!result)
        {
            if (_autoHelp)
            {
                ShowHelp(argv[0]);
            }
            return false;
        }
    }
    catch (const exception & e)
    {
        _console << fgcolor(Tools::ConsoleColor::Red) << "Exception: " << e.what() << endl;
        if (_autoHelp)
        {
            ShowHelp(argv[0]);
        }
        return false;
    }
    return true;
}

bool CommandLineParser::InternalParse(int argc, const char * argv[])
{
    size_t argCount = size_t(argc);
    FillOptionsList();
    string optionString;
    for (auto opt : _options)
    {
        if (!opt->IsSwitchWithVariable())
        {
            if (opt->ShortName())
            {
                optionString += opt->ShortName();
                if (opt->ArgType() != CommandLineArgumentType::NoArgument)
                    optionString += ':';
                if (opt->ArgType() == CommandLineArgumentType::OptionalArgument)
                    optionString += ':';
            }
        }
    }

    int result = 0;
    size_t optionIndex;
    _getOptData.Initialize();
    while (result != -1)
    {
        result = GetOpt(static_cast<size_t>(argc), argv, optionString, optionIndex);
        if (result == 0)
        {
            CommandLineOption::Ptr option = _options[optionIndex];
            auto optionWithVariable = dynamic_pointer_cast<CommandLineSwitchWithVariable<int>>(option);
            if (optionWithVariable)
            {
                SelectOption(option, nullptr);
            } else
            {
                SelectOption(option, _getOptData.optionArgument);
            }
        } else if (result == '?')
        {
            break;
        } else if (result != -1)
        {
            size_t index = MatchShortOption(static_cast<char>(result));
            if (index < numeric_limits<size_t>::max())
            {
                CommandLineOption::Ptr option = _options[index];
                switch (option->ArgType())
                {
                    case CommandLineArgumentType::NoArgument:
                        SelectOption(option, nullptr);
                        break;
                    case CommandLineArgumentType::OptionalArgument:
                        SelectOption(option, _getOptData.optionArgument);
                        break;
                    case CommandLineArgumentType::RequiredArgument:
                        SelectOption(option, _getOptData.optionArgument);
                        break;
                    default:
                        break;
                }
            }
        }

    }

    while (_getOptData.optionIndex < argCount)
    {
        AddNonOption(argv[_getOptData.optionIndex]);
        _getOptData.optionIndex++;
    }

    return (result != '?');
}

void CommandLineParser::ExchangeOption(const char ** argv)
{
    size_t bottom = _getOptData.firstNonOption;
    size_t middle = _getOptData.lastNonOption;
    size_t top = _getOptData.optionIndex;
    const char * tmp;
    while ((top > middle) && (middle > bottom))
    {
        if ((top - middle) > (middle - bottom))
        {
            size_t len = middle - bottom;
            for (size_t i = 0; i < len; i++)
            {
                tmp = argv[bottom + i];
                argv[bottom + i] = argv[top - (middle - bottom) + i];
                argv[top - (middle - bottom) + i] = tmp;
            }
            top -= len;
        }
        else
        {
            size_t len = top - middle;
            for (size_t i = 0; i < len; i++)
            {
                tmp = argv[bottom + i];
                argv[bottom + i] = argv[middle + i];
                argv[middle + i] = tmp;
            }
            bottom += len;
        }
    }
    _getOptData.firstNonOption += (_getOptData.optionIndex - _getOptData.lastNonOption);
    _getOptData.lastNonOption = _getOptData.optionIndex;
}

bool CommandLineParser::AtNonOption(size_t argCount, const char ** argv) const
{
    return (_getOptData.optionIndex < argCount) &&
           (('-' != argv[_getOptData.optionIndex][0]) || ('\0' == argv[_getOptData.optionIndex][1]));
}

bool CommandLineParser::AtLongOption(size_t argCount, const char ** argv) const
{
    return (_getOptData.optionIndex < argCount) &&
           (('-' == argv[_getOptData.optionIndex][0]) && ('-' == argv[_getOptData.optionIndex][1]));
}

int CommandLineParser::HandleLongOption(size_t argCount, const char ** argv, const char * optionString,
                                        size_t & optionIndex, bool printErrors)
{
    // Long option
    const char * nameEnd;
    size_t nameLength;
    CommandLineOption::Ptr optionFound = nullptr;
    struct OptionList
    {
        CommandLineOption::Ptr option;
        struct OptionList * next;
    } * amiguousOptionsList = nullptr;

    bool exact = false;
    size_t indexFound {};
    for (nameEnd = _getOptData.nextChar; *nameEnd && ('=' != *nameEnd); nameEnd++)
        ;

    nameLength = static_cast<size_t>(nameEnd - _getOptData.nextChar);
    size_t currentOptionIndex = 0;
    for (auto currentOption : _options)
    {
        if (!strncmp(currentOption->LongName().c_str(), _getOptData.nextChar, nameLength))
        {
            auto currentOptionWithVariable = dynamic_pointer_cast<CommandLineSwitchWithVariable<int>>(currentOption);
            auto optionFoundWithVariable = dynamic_pointer_cast<CommandLineSwitchWithVariable<int>>(optionFound);
            if (nameLength == currentOption->LongName().length())
            {
                optionFound = currentOption;
                indexFound = currentOptionIndex;
                exact = true;
                break;
            } else if (nullptr == optionFound)
            {
                optionFound = currentOption;
                indexFound = currentOptionIndex;
            } else if (currentOption->IsNotEqual(optionFound))
            {
                struct OptionList *newOption = new OptionList;
                newOption->option = currentOption;
                newOption->next = amiguousOptionsList;
                amiguousOptionsList = newOption;
            }
        }
        currentOptionIndex++;
    }
    if ((nullptr != amiguousOptionsList) && !exact)
    {
        if (printErrors)
        {
            struct OptionList first;
            first.option = optionFound;
            first.next = amiguousOptionsList;
            amiguousOptionsList = &first;
            _console << fgcolor(Tools::ConsoleColor::Red) << argv[0] << ": option '" << argv[_getOptData.optionIndex] << "' is ambiguous; possibilities:";
            do
            {
                _console << " '--" << amiguousOptionsList->option->LongName() << "'";
                amiguousOptionsList = amiguousOptionsList->next;
            }
            while (nullptr != amiguousOptionsList);
            _console << fgcolor(Tools::ConsoleColor::Default) << endl;
        }
        _getOptData.nextChar += strlen(_getOptData.nextChar);
        _getOptData.optionIndex++;
        _getOptData.optopt = 0;
        return '?';
    }
    if (nullptr != optionFound)
    {
        currentOptionIndex = indexFound;
        _getOptData.optionIndex++;
        if (*nameEnd)
        {
            // We have an argument after the equals sign.
            if (CommandLineArgumentType::NoArgument != optionFound->ArgType())
                _getOptData.optionArgument = nameEnd + 1;
            else
            {
                if (printErrors)
                {
                    _console << fgcolor(Tools::ConsoleColor::Red);
                    if ('-' == argv[_getOptData.optionIndex - 1][1])
                    {
                        _console << argv[0] << ": option '--" << optionFound->LongName() << "' doesn't allow an argument";
                    }
                    else
                    {
                        _console << argv[0] << ": option '--" << argv[_getOptData.optionIndex - 1][0] << optionFound->LongName() << "' doesn't allow an argument";
                    }
                    _console << fgcolor(Tools::ConsoleColor::Default) << endl;
                }
                _getOptData.nextChar += strlen(_getOptData.nextChar);
                auto optionFoundWithVariable = dynamic_pointer_cast<CommandLineSwitchWithVariable<int>>(optionFound);
                _getOptData.optopt = optionFound->ShortName();
                if (optionFoundWithVariable)
                {
                    _getOptData.optopt = optionFoundWithVariable->Value();
                }
                return '?';
            }
        }
        else if (CommandLineArgumentType::RequiredArgument == optionFound->ArgType())
        {
            if (_getOptData.optionIndex < argCount)
                _getOptData.optionArgument = argv[_getOptData.optionIndex++];
            else
            {
                if (printErrors)
                {
                    _console << fgcolor(Tools::ConsoleColor::Red);
                    _console <<  argv[0] << ": option '--" << optionFound->LongName() << "' requires an argument";
                    _console << fgcolor(Tools::ConsoleColor::Default) << endl;
                }
                _getOptData.nextChar += strlen(_getOptData.nextChar);
                _getOptData.optopt = optionFound->ShortName();
                return optionString[0] == ':' ? ':' : '?';
            }
        }
        _getOptData.nextChar += strlen(_getOptData.nextChar);
        optionIndex = currentOptionIndex;
        optionFound->SetOptionFound();
        if (optionFound->IsSwitchWithVariable())
        {
            return 0;
        }
        return optionFound->ShortName();
    }
    if (printErrors)
    {
        _console << fgcolor(Tools::ConsoleColor::Red);
        if ('-' == argv[_getOptData.optionIndex][1])
        {
            _console <<  argv[0] << ": unrecognized option '--" << _getOptData.nextChar << "'";
        }
        else
        {
            _console <<  argv[0] << ": unrecognized option '--" << argv[_getOptData.optionIndex][0] << _getOptData.nextChar << "'";
        }
        _console << fgcolor(Tools::ConsoleColor::Default) << endl;
    }
    _getOptData.nextChar = "";
    _getOptData.optionIndex++;
    _getOptData.optopt = 0;
    return '?';
}

void CommandLineParser::SetArgument()
{
    if ('=' == *_getOptData.nextChar)
        _getOptData.nextChar++;
    _getOptData.optionArgument = _getOptData.nextChar;
    _getOptData.optionIndex++;
}

int CommandLineParser::HandleShortOption(size_t argCount, const char ** argv, const char * optionString,
                                         bool printErrors)
{
    char c = *_getOptData.nextChar++;
    const char * foundShortOption = strchr(optionString, c);
    if ('\0' == *_getOptData.nextChar)
        // No argument in this command line parameter
        ++_getOptData.optionIndex;
    if ((nullptr == foundShortOption) || (':' == c))
    {
        if (printErrors)
        {
            _console << fgcolor(Tools::ConsoleColor::Red);
            _console <<  argv[0] << ": invalid option -- '" << c << "'";
            _console << fgcolor(Tools::ConsoleColor::Default) << endl;
        }
        _getOptData.optopt = c;
        return '?';
    }
    if (foundShortOption[1] == ':')
    {
        // We could expect an argument
        if (foundShortOption[2] == ':')
        {
            // The argument is optional
            if ('\0' != *_getOptData.nextChar)
                // We have an argument
                SetArgument();
            else
                // No argument
                _getOptData.optionArgument = nullptr;
            _getOptData.nextChar = nullptr;
        }
        else
        {
            // The argument is required
            if ('\0' != *_getOptData.nextChar)
                // We have an argument in this command line parameter
                SetArgument();
            else if (_getOptData.optionIndex == argCount)
            {
                // No argument
                if (printErrors)
                {
                    _console << fgcolor(Tools::ConsoleColor::Red);
                    _console <<  argv[0] << ": option '--" << c << "' requires an argument";
                    _console << fgcolor(Tools::ConsoleColor::Default) << endl;
                }
                _getOptData.optopt = c;
                c = (optionString[0] == ':') ? ':' : '?';
            }
            else
                // We have an argument in next command line parameter
                _getOptData.optionArgument = argv[_getOptData.optionIndex++];
            _getOptData.nextChar = nullptr;
        }
    }
    return c;
}

int CommandLineParser::GetOpt(size_t argCount, const char ** argv, const string & optionString,
                              size_t & optionIndex)
{
    if (argCount < 1)
        return -1;

    const char * optionStringPtr = optionString.c_str();
    bool printErrors = _getOptData.printErrors;
    _getOptData.optionArgument = nullptr;

    if (('-' == *optionStringPtr) || ('+' == *optionStringPtr))
        optionStringPtr++;

    if (*optionStringPtr == ':')
        printErrors = false;
    if ((nullptr == _getOptData.nextChar) || ('\0' == *_getOptData.nextChar))
    {
        _getOptData.lastNonOption = min(_getOptData.lastNonOption, _getOptData.optionIndex);
        _getOptData.firstNonOption = min(_getOptData.firstNonOption, _getOptData.optionIndex);
        if (CommandLineParser::Ordering::Permute == _getOptData.ordering)
        {
            if ((_getOptData.firstNonOption != _getOptData.lastNonOption) &&
                (_getOptData.lastNonOption != _getOptData.optionIndex))
                ExchangeOption(argv);
            else if (_getOptData.lastNonOption != _getOptData.optionIndex)
                _getOptData.firstNonOption = _getOptData.optionIndex;
            // Skip non-_options and empty _options
            while (AtNonOption(argCount, argv))
                _getOptData.optionIndex++;
            _getOptData.lastNonOption = _getOptData.optionIndex;
        }
        if ((_getOptData.optionIndex < argCount ) && !strcmp(argv[_getOptData.optionIndex], "--"))
        {
            // We are apparently using stdin as input?
            _getOptData.optionIndex++;
            if ((_getOptData.firstNonOption != _getOptData.lastNonOption) &&
                (_getOptData.lastNonOption != _getOptData.optionIndex))
                ExchangeOption(argv);
            else if (_getOptData.firstNonOption == _getOptData.lastNonOption)
                _getOptData.firstNonOption = _getOptData.optionIndex;
            _getOptData.lastNonOption = _getOptData.optionIndex = argCount;
        }
        if (_getOptData.optionIndex == argCount)
        {
            // Point to first non-option
            if (_getOptData.firstNonOption != _getOptData.lastNonOption)
                _getOptData.optionIndex = _getOptData.firstNonOption;
            return -1;
        }
        if (AtNonOption(argCount, argv))
        {
            if (_getOptData.ordering == CommandLineParser::Ordering::RequireOrder)
                // There ar no more options
                return -1;
            _getOptData.optionArgument = argv[_getOptData.optionIndex++];
            return 1;
        }
        _getOptData.nextChar = (argv[_getOptData.optionIndex] + 1 +
                                (('-' == argv[_getOptData.optionIndex][1])? 1 : 0));
    }
    if (AtLongOption(argCount, argv))
    {
        return HandleLongOption(argCount, argv, optionStringPtr, optionIndex, printErrors);
    }
    return HandleShortOption(argCount, argv, optionStringPtr, printErrors);
}

void CommandLineParser::ShowHelp(const string & applicationName)
{
    _console << fgcolor(Tools::ConsoleColor::Yellow) << GetHelp(applicationName)
             << endl;
    _console << fgcolor(Tools::ConsoleColor::Default);
}

void CommandLineParser::AddOption(const CommandLineOption::Ptr option)
{
    _mainGroup->AddOption(option);
}

size_t CommandLineParser::MatchShortOption(char name) const
{
    for (size_t i = 0; i < _options.size(); i++)
    {
        if (_options[i]->ShortName() == name)
        {
            return i;
        }
    }
    return numeric_limits<size_t>::max();
}

size_t CommandLineParser::MatchLongOption(const string & name) const
{
    return MatchLongOption(name.c_str(), name.length());
}

size_t CommandLineParser::MatchLongOption(const char * name, size_t nameLength) const
{
    for (size_t i = 0; i < _options.size(); i++)
    {
        // find matching long option
        if (_options[i]->LongName() == string(name, nameLength))
        {
            // exact match
            return i;
        }
        // If this is a known short option, don't allow a partial match of a single character.
        if ((nameLength == 1) && (*name == _options[i]->ShortName()))
            return i;
    }
    return numeric_limits<size_t>::max();
}

void CommandLineParser::SelectOption(CommandLineOption::Ptr option, const char * value)
{
    option->SetOptionFound();
    string argument = (value != nullptr) ? value : "";
    argument = Trim(argument, "\"");
    switch (option->ArgType())
    {
        case CommandLineArgumentType::NoArgument:
            break;
        case CommandLineArgumentType::OptionalArgument:
            if (value != nullptr)
                option->Argument(argument);
            break;
        case CommandLineArgumentType::RequiredArgument:
            option->Argument(argument);
            break;
    }
    OnParseOption(option);
}

void CommandLineParser::AddNonOption(const char * value)
{
    _nonOptions.push_back(string(Trim(value, "\"")));
    OnParseNonOption(value);
}

void CommandLineParser::AddGroup(const CommandLineOptionGroup::Ptr group)
{
    _groups.push_back(group);
}

void CommandLineParser::MainGroup(const CommandLineOptionGroup::Ptr group)
{
    if (!IsInGroups(group))
    {
        AddGroup(group);
    }
    _mainGroup = group;
}

bool CommandLineParser::IsInGroups(const CommandLineOptionGroup::Ptr group) const
{
    for (auto groupFound : _groups)
    {
        if (groupFound == group)
            return true;
    }
    return false;
}

string CommandLineParser::GetHelp(const string & applicationName, bool mainHelp /* = true*/) const
{
    return GetHelp(applicationName, mainHelp, CommandLineOptionGroup::Ptr());
}

string CommandLineParser::GetHelp(const string & applicationName, bool mainHelp,
                                        const CommandLineOptionGroup::Ptr group) const
{
    string strippedPath = LastPartOfPath(applicationName);
    const int indentOptions = 2;
    const int indentDescription = 25;
    basic_ostringstream<char> stream;
    stream << "Usage:" << endl;
    stream << _name << ": " << _description << endl << endl;
    stream << string(indentOptions, ' ') << strippedPath << " [OPTION...]" << endl << endl;
    for (auto foundGroup : _groups)
    {
        if ((group != nullptr) && (group != foundGroup))
            continue;
        if (_options.size() > 0)
        {
            for (auto option : foundGroup->Options())
            {
                string optionsText = (option->ShortName() != 0)
                                           ? string("-") + option->ShortName() + ", "
                                           : "";
                optionsText += "--" + option->LongName();
                if (option->ArgType() == CommandLineArgumentType::OptionalArgument)
                {
                    optionsText += " [argument]";
                } else if (option->ArgType() == CommandLineArgumentType::RequiredArgument)
                {
                    optionsText += " <argument>";
                }
                stream << string(indentOptions, ' ') << optionsText;
                if (optionsText.length() < indentDescription)
                    stream << string(indentDescription - optionsText.length(), ' ');
                stream << " " << option->Description() << endl;
            }
            stream << endl;
        }
    }
    return stream.str();
}

void CommandLineParser::FillOptionsList()
{
    for (auto group : _groups)
    {
        for (auto option : group->Options())
        {
            _options.push_back(option);
        }
    }
}

void CommandLineParser::SetupStandardGroups()
{
    CommandLineOptionGroup::Ptr mainGroup(new CommandLineOptionGroup(_name, _description));
    AddGroup(mainGroup);

    CommandLineOptionGroup::Ptr helpGroup(new CommandLineOptionGroup(HelpOptionsGroupName));
    AddGroup(helpGroup);
    helpGroup->AddSwitchWithVariable("help", _showHelp, true, "Show help options");

    MainGroup(mainGroup);
}

