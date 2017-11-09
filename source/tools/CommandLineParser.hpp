#pragma once

#include <string>
#include <vector>
#include "tools/Definitions.hpp"
#include "tools/CommandLineOption.hpp"
#include "tools/CommandLineOptionGroup.hpp"
#include "tools/Console.hpp"

namespace Tools {

class CommandLineParser
{
private:
    using CommandLineOptionsList = std::vector<CommandLineOption::Ptr>;
    using CommandLineNonOptionsList = std::vector<std::string>;
    static const std::string DefaultMainOptionsGroupName;
    static const std::string HelpOptionsGroupName;

public:
    CommandLineParser(Console & console,
                      const std::string & name = DefaultMainOptionsGroupName,
                      const std::string & description = "");
    CommandLineParser(const CommandLineParser &) = delete;
    CommandLineParser & operator = (const CommandLineParser &) = delete;

    virtual ~CommandLineParser();

    void AddSwitch(const std::string &longName, char shortName,
                   const std::string &description)
    {
        AddOption(std::make_shared<CommandLineSwitchNoVariable>(longName, shortName, description));
    }

    template <class T>
    void AddSwitchWithVariable(const std::string &longName, T &variable, T value,
                               const std::string &description)
    {
        AddOption(std::make_shared<CommandLineSwitchWithVariable<T>>(longName, description, variable, value));
    }

    void AddOptionOptionalArgument(const std::string & longName, char shortName,
                                   const std::string & description, std::string & textVariable)
    {
        AddOption(std::make_shared<CommandLineOption>(longName, shortName, description, textVariable,
                                                      CommandLineArgumentType::OptionalArgument));
    }

    void AddOptionOptionalArgument(const std::string & longName,
                                   const std::string & description, std::string & textVariable)
    {
        AddOption(std::make_shared<CommandLineOption>(longName, '\0', description, textVariable,
                                                      CommandLineArgumentType::OptionalArgument));
    }

    template<class T>
    void AddOptionOptionalArgument(const std::string & longName, char shortName,
                                   const std::string & description, T & variable)
    {
        AddOption(std::make_shared<CommandLineOptionWithVariable<T>>(longName, shortName, description,
                                                                     variable,
                                                                     CommandLineArgumentType::OptionalArgument));
    }

    template<class T>
    void AddOptionOptionalArgument(const std::string & longName,
                                   const std::string & description, T & variable)
    {
        AddOption(std::make_shared<CommandLineOptionWithVariable<T>>(longName, '\0', description,
                                                                     variable,
                                                                     CommandLineArgumentType::OptionalArgument));
    }

    void AddOptionRequiredArgument(std::string longName, char shortName,
                                   const std::string & description, std::string & textVariable)
    {
        AddOption(std::make_shared<CommandLineOption>(longName, shortName, description, textVariable,
                                                      CommandLineArgumentType::RequiredArgument));
    }

    void AddOptionRequiredArgument(std::string longName,
                                   const std::string & description, std::string & textVariable)
    {
        AddOption(std::make_shared<CommandLineOption>(longName, '\0', description, textVariable,
                                                      CommandLineArgumentType::RequiredArgument));
    }

    template<class T>
    void AddOptionRequiredArgument(const std::string & longName, char shortName,
                                   const std::string & description, T & variable)
    {
        AddOption(std::make_shared<CommandLineOptionWithVariable<T>>(longName, shortName, description,
                                                                     variable,
                                                                     CommandLineArgumentType::RequiredArgument));
    }

    template<class T>
    void AddOptionRequiredArgument(const std::string & longName,
                                   const std::string & description, T & variable)
    {
        AddOption(std::make_shared<CommandLineOptionWithVariable<T>>(longName, '\0', description,
                                                                     variable,
                                                                     CommandLineArgumentType::RequiredArgument));
    }

    size_t NumNonOptions() const
    {
        return _nonOptions.size();
    }

    std::string GetNonOption(size_t index) const
    {
        if (index < _nonOptions.size())
            return _nonOptions[index];
        return {};
    }

    bool HaveOption(char shortName) const;
    bool HaveOption(std::string longName) const;
    bool FoundOption(char shortName) const;
    bool FoundOption(std::string longName) const;
    std::string GetOption(char shortName) const;
    std::string GetOption(std::string longName) const;

    void AddOption(const CommandLineOption::Ptr option);

    void AddGroup(const CommandLineOptionGroup::Ptr group);
    const CommandLineOptionGroup::Ptr MainGroup() const { return _mainGroup; }
    void MainGroup(const CommandLineOptionGroup::Ptr group);
    const CommandLineOptionGroup::List & Groups() const { return _groups; }
    bool IsInGroups(const CommandLineOptionGroup::Ptr group) const;

    bool Parse(int argc, const char * argv[]);

    bool AutoHandleHelp() const
    {
        return _autoHelp;
    }
    void AutoHandleHelp(bool autoHelp)
    {
        this->_autoHelp = autoHelp;
    }
    bool ShouldShowHelp() const
    {
        return _showHelp;
    }
    std::string GetHelp(const std::string & applicationName, bool mainHelp = true) const;
    std::string GetHelp(const std::string & applicationName, bool mainHelp,
                         const CommandLineOptionGroup::Ptr group) const;

    virtual void OnParseOption(const CommandLineOption::Ptr option PARAMETER_NOT_USED)
    {
//            _console << "Option " << option->LongName() << " argument " << option->Argument() << std::endl;
    }
    virtual void OnParseNonOption(const std::string parameter PARAMETER_NOT_USED)
    {
//            _console << "Non-option " << parameter << std::endl;
    }

    enum class Ordering { RequireOrder, Permute, ReturnInOrder };
    class GetOptData
    {
    public:
        GetOptData()
            : optionIndex()
            , printErrors(true)
            , optopt()
            , optionArgument()
            , nextChar()
            , ordering()
            , firstNonOption()
            , lastNonOption()
        {
        }
        void Initialize()
        {
            optionIndex = firstNonOption = lastNonOption = 1;
            nextChar = nullptr;
            ordering = CommandLineParser::Ordering::Permute;
        }
        size_t optionIndex;
        bool printErrors;
        int optopt;
        const char * optionArgument;
        bool initialized;
        const char * nextChar;
        Ordering ordering;
        size_t firstNonOption;
        size_t lastNonOption;
    };

protected:
    CommandLineOptionGroup::List _groups;
    CommandLineOptionGroup::Ptr _mainGroup;
    Console _console;

private:
    bool _autoHelp;
    bool _showHelp;
    std::string _name;
    std::string _description;
    CommandLineNonOptionsList _nonOptions;
    CommandLineOption::List _options;
    GetOptData _getOptData;

    bool InternalParse(int argc, const char * argv[]);
    void ExchangeOption(const char ** argv);
    bool AtNonOption(size_t argCount, const char ** argv) const;
    bool AtLongOption(size_t argCount, const char ** argv) const;
    int HandleLongOption(size_t argCount, const char ** argv, const char * optionString,
                         size_t & optionIndex, bool printErrors);
    void SetArgument();
    int HandleShortOption(size_t argCount, const char ** argv, const char * optionString,
                          bool printErrors);
    int GetOpt(size_t argCount, const char ** argv, const std::string & optionString,
               size_t & optionIndex);
    void ShowHelp(const std::string & applicationName);
    size_t MatchShortOption(char name) const;
    size_t MatchLongOption(const std::string & name) const;
    size_t MatchLongOption(const char * name, size_t nameLength) const;
    void SelectOption(CommandLineOption::Ptr option, const char * value);
    void AddNonOption(const char * value);

    void FillOptionsList();
    void SetupStandardGroups();
};

} // namespace Tools