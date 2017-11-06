#pragma once

#include <memory>
#include <vector>
#include "tools/CommandLineOption.hpp"

namespace Tools
{

typedef std::vector<CommandLineOption::Ptr> CommandLineOptionsList;

class CommandLineOptionGroup
{
public:
    using Ptr = std::shared_ptr<CommandLineOptionGroup>;
    using List = std::vector<Ptr>;

    CommandLineOptionGroup() = delete;
    CommandLineOptionGroup(const std::string & name, const std::string description = "");
    CommandLineOptionGroup(const CommandLineOptionGroup &) = delete;
    virtual ~CommandLineOptionGroup();

    CommandLineOptionGroup & operator = (const CommandLineOptionGroup &) = delete;

    const std::string & Name() const
    {
        return _name;
    }
    void Name(const std::string & name)
    {
        this->_name = name;
    }
    const std::string & Description() const
    {
        return _description;
    }
    void Description(const std::string & description)
    {
        this->_description = description;
    }

    void AddOption(const CommandLineOption::Ptr option);
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
    void AddOptionOptionalArgumentWithVariable(const std::string & longName, char shortName,
                                               const std::string & description, T & variable)
    {
        AddOption(std::make_shared<CommandLineOptionWithVariable<T>>(longName, shortName, description, variable,
                                                                  CommandLineArgumentType::OptionalArgument));
    }

    template<class T>
    void AddOptionOptionalArgumentWithVariable(const std::string & longName,
                                               const std::string & description, T & variable)
    {
        AddOption(std::make_shared<CommandLineOptionWithVariable<T>>(longName, '\0', description, variable,
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
    void AddOptionRequiredArgumentWithVariable(const std::string & longName, char shortName,
                                               const std::string & description, T & variable)
    {
        AddOption(std::make_shared<CommandLineOptionWithVariable<T>>(longName, shortName, description, variable,
                                                                  CommandLineArgumentType::RequiredArgument));
    }

    template<class T>
    void AddOptionRequiredArgumentWithVariable(const std::string & longName,
                                               const std::string & description, T & variable)
    {
        AddOption(std::make_shared<CommandLineOptionWithVariable<T>>(longName, '\0', description, variable,
                                                                  CommandLineArgumentType::RequiredArgument));
    }

    const CommandLineOptionsList & Options() const
    {
        return _options;
    }

protected:
	std::string _name;
	std::string _description;
    CommandLineOptionsList _options;
};

} // namespace Tools
