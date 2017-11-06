#pragma once

#include <iostream>
#include <memory>
#include "tools/Serialization.hpp"
#include "tools/DeserializationImpl.hpp"
#include "tools/SerializationImpl.hpp"

namespace Tools {


enum class CommandLineArgumentType
{
    NoArgument,
    RequiredArgument,
    OptionalArgument,
};
inline std::ostream & operator << (std::ostream & stream, const CommandLineArgumentType & value)
{
    stream << Serialize(value);
    return stream;
}

class CommandLineOption
{
public:
    using Ptr = std::shared_ptr<CommandLineOption>;
    using List = std::vector<Ptr>;

    CommandLineOption() = delete;
    CommandLineOption(const CommandLineOption & other) = delete;
    CommandLineOption & operator = (const CommandLineOption & other) = delete;

    CommandLineOption(const std::string & longName, char shortName, const std::string & description,
                      CommandLineArgumentType argType = CommandLineArgumentType::NoArgument);
    CommandLineOption(const std::string & longName, char shortName, const std::string & description,
                      std::string & textVariable,
                      CommandLineArgumentType argType = CommandLineArgumentType::RequiredArgument);
    virtual ~CommandLineOption() {}

public:
    const std::string & LongName() const { return _longName; }
    char ShortName() const { return _shortName; }
    const std::string & Description() const { return _description; }
    CommandLineArgumentType ArgType() const { return _argType; }
    virtual bool IsSwitchNoVariable() const { return false; }
    virtual bool IsSwitchWithVariable() const  { return false; }
    virtual bool IsOptionWithVariable() const  { return false; }
    virtual bool IsEqual(Ptr other) const
    {
        return ArgType() == other->ArgType();
    }
    virtual bool IsNotEqual(Ptr other) const { return !IsEqual(other); }
    std::string TextValue() const
    {
        if (_textValue)
            return *_textValue;
        return {};
    }
    void TextValue(const std::string & value)
    {
        if (_textValue)
            *_textValue = value;
    }
    bool FoundOption() const { return _optionFound; }
    virtual void SetOptionFound()
    {
        _optionFound = true;
    }
    std::string Argument() { return _argument; }
    virtual void Argument(const std::string & value)
    {
        _argument = value;
        if (nullptr != _textValue)
            *_textValue = value;
    }

protected:
    void LongName(const std::string & name) { _longName = name; }
    void ShortName(char name) { _shortName = name; }
    void Description(const std::string & description) { this->_description = description; }

private:
    std::string _longName;
    char _shortName;
    std::string _description;
    CommandLineArgumentType _argType;
    std::string * _textValue;
    bool _optionFound;
    std::string _argument;
};

class CommandLineSwitchNoVariable : public CommandLineOption
{
public:
    CommandLineSwitchNoVariable() = delete;
    CommandLineSwitchNoVariable(const CommandLineSwitchNoVariable &) = delete;
    CommandLineSwitchNoVariable & operator = (const CommandLineSwitchNoVariable &) = delete;

    CommandLineSwitchNoVariable(const std::string & longName, const char shortName, const std::string & description)
        : CommandLineOption(longName, shortName, description, CommandLineArgumentType::NoArgument)
    {
    }
    virtual bool IsSwitchNoVariable() const override { return true; }

private:
};

template <class T, class Deserializer = StringDeserializer<T>>
class CommandLineSwitchWithVariable : public CommandLineOption
{
public:
    CommandLineSwitchWithVariable() = delete;
    CommandLineSwitchWithVariable(const CommandLineSwitchWithVariable &) = delete;
    CommandLineSwitchWithVariable & operator = (const CommandLineSwitchWithVariable &) = delete;

    CommandLineSwitchWithVariable(const std::string & longName, const std::string & description,
                                  T & variable, T value)
        : CommandLineOption(longName, '\0', description, CommandLineArgumentType::NoArgument)
        , _variable(variable)
        , _value(value)
    {
    }

    virtual void SetOptionFound() override
    {
        _variable = _value;
        CommandLineOption::SetOptionFound();
    }
    virtual bool IsSwitchWithVariable() const override { return true; }

    T & Variable() { return _variable; }
    T Value() const { return _value; }
    virtual bool IsEqual(Ptr other) const override
    {
        if (ArgType() != other->ArgType())
            return false;
        auto otherSwitchWithVariable = std::dynamic_pointer_cast<CommandLineSwitchWithVariable>(other);
        if (nullptr == otherSwitchWithVariable)
            return false;
        return (&_variable == &(otherSwitchWithVariable->_variable)) &&
               (Value() == otherSwitchWithVariable->Value());
    }

private:
    T & _variable;
    T _value;
};

template <class T, class Deserializer = StringDeserializer<T>>
class CommandLineOptionWithVariable : public CommandLineOption
{
public:
    CommandLineOptionWithVariable() = delete;
    CommandLineOptionWithVariable(const CommandLineOptionWithVariable &) = delete;
    CommandLineOptionWithVariable & operator = (const CommandLineOptionWithVariable &) = delete;

    CommandLineOptionWithVariable(const std::string & longName, char shortName, const std::string & description,
                                  T & variable,
                                  CommandLineArgumentType argType = CommandLineArgumentType::RequiredArgument)
        : CommandLineOption(longName, shortName, description, argType)
        , _variable(variable)
    {
    }

    virtual void Argument(const std::string & value)
    {
        StringDeserializer<T> deserializer;
        deserializer.Deserialize(value, _variable);
        CommandLineOption::Argument(value);
    }
    virtual bool IsOptionWithVariable() const override { return true; }

    T & Variable() { return _variable; }
    virtual bool IsEqual(Ptr other) const override
    {
        if (ArgType() != other->ArgType())
            return false;
        auto otherSwitchWithVariable = std::dynamic_pointer_cast<CommandLineOptionWithVariable>(other);
        if (nullptr == otherSwitchWithVariable)
            return false;
        return (&_variable == &(otherSwitchWithVariable->_variable));
    }

private:
    T & _variable;
};

} // namespace Tools