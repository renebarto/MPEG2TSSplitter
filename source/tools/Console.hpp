#pragma once

#include <iostream>

namespace Tools
{

class Secret;

} // namespace Tools

// Ensures that there is at least one operator<< in the global namespace.
// See Message& operator<<(...) below for why.
void operator <<(const Tools::Secret&, int);

namespace Tools
{

enum class ConsoleColor : int16_t
{
    Default = -1,
    Black = 0,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,
    ColorMask = 0x07,
    Intensity = 0x08,
    Bold = 0x10,
};

std::ostream & operator << (std::ostream & stream, ConsoleColor value);

struct _SetForegroundColor
{
    ConsoleColor color;
};

struct _SetBackgroundColor
{
    ConsoleColor color;
};

class Console
{
private:
    // The type of basic IO manipulators (endl, ends, and flush) for narrow
    // streams.
    typedef std::ostream & (* BasicIoManip)(std::ostream &);

public:
    static const int InvalidHandle = -1;

    Console(int handle = fileno(stdout));
    explicit Console(std::ostream & stream);

    void SetForegroundColor(ConsoleColor foregroundColor);
    void SetBackgroundColor(ConsoleColor backgroundColor);
    void SetTerminalColor(ConsoleColor foregroundColor = ConsoleColor::Default,
                          ConsoleColor backgroundColor = ConsoleColor::Default);
    void ResetTerminalColor();
    bool ShouldUseColor();

    // Streams a non-pointer _value to this object.
    template <typename T>
    inline Console & operator << (const T & val)
    {
        using ::operator <<;
        if (_stream)
            *_stream << val;
        return *this;
    }

    Console & operator << (BasicIoManip val)
    {
        if (_stream)
            *_stream << val;
        return *this;
    }

    Console & operator << (_SetForegroundColor color);
    Console & operator << (_SetBackgroundColor color);

protected:
    std::ostream * _stream;
    int _handle;
    ConsoleColor _currentForegroundColor;
    ConsoleColor _currentBackgroundColor;
};

Tools::_SetForegroundColor fgcolor(Tools::ConsoleColor color);
Tools::_SetBackgroundColor bgcolor(Tools::ConsoleColor color);

inline void Console::SetForegroundColor(ConsoleColor foregroundColor)
{
    SetTerminalColor(foregroundColor, _currentBackgroundColor);
}

inline void Console::SetBackgroundColor(ConsoleColor backgroundColor)
{
    SetTerminalColor(_currentForegroundColor, backgroundColor);
}

inline void Console::ResetTerminalColor()
{
    SetTerminalColor();
}

inline Console & Console::operator<<(_SetForegroundColor color)
{
    SetForegroundColor(color.color);
    return *this;
}

inline Console & Console::operator<<(_SetBackgroundColor color)
{
    SetBackgroundColor(color.color);
    return *this;
}

Console & DefaultConsole();

} // namespace Tools
