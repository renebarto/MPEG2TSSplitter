#include <unistd.h>
#include <iostream>
#include "tools/Console.hpp"

using namespace std;

namespace Tools {

inline ostream * DetermineStream(int handle)
{
    switch (handle)
    {
        case STDOUT_FILENO:
            return &cout;
        case STDERR_FILENO:
            return &cerr;
        case STDIN_FILENO:
        default:
            cerr << "Invalid handle specified, please specify only stdout or stderr handle" << endl;
    }
    return nullptr;
}

inline int DetermineHandle(ostream * stream)
{
    if (stream == &cout)
    {
        return fileno(stdout);
    }
    else if (stream == &cerr)
    {
        return fileno(stderr);
    }
    return -1;
}

inline const char * GetAnsiColorCode(ConsoleColor color)
{
    switch (static_cast<ConsoleColor>(static_cast<int16_t>(color) & static_cast<int16_t>(ConsoleColor::ColorMask)))
    {
        case ConsoleColor::Black:
            return "0";
        case ConsoleColor::Red:
            return "1";
        case ConsoleColor::Green:
            return "2";
        case ConsoleColor::Yellow:
            return "3";
        case ConsoleColor::Blue:
            return "4";
        case ConsoleColor::Magenta:
            return "5";
        case ConsoleColor::Cyan:
            return "6";
        case ConsoleColor::White:
            return "7";
        default:
            return nullptr;
    };
}

_SetForegroundColor fgcolor(ConsoleColor color)
{
    return{ color };
}

_SetBackgroundColor bgcolor(ConsoleColor color)
{
    return{ color };
}

Console::Console(int handle)
    : _stream(DetermineStream(handle))
    , _handle(handle)
    , _currentForegroundColor(ConsoleColor::Default)
    , _currentBackgroundColor(ConsoleColor::Default)
{
}

Console::Console(ostream & stream)
    : _stream(&stream)
    , _handle(DetermineHandle(&stream))
    , _currentForegroundColor(ConsoleColor::Default)
    , _currentBackgroundColor(ConsoleColor::Default)
{
}

void Console::SetTerminalColor(ConsoleColor foregroundColor, ConsoleColor backgroundColor)
{
    if (!ShouldUseColor())
        return;
    string command = "\033[0";
    if (foregroundColor != ConsoleColor::Default)
    {
        if ((static_cast<ConsoleColor>(static_cast<int16_t>(foregroundColor) &
                                       static_cast<int16_t>(ConsoleColor::Bold))) == ConsoleColor::Bold)
        {
            command += ";1";
        }
        if ((static_cast<ConsoleColor>(static_cast<int16_t>(foregroundColor) &
                                       static_cast<int16_t>(ConsoleColor::Intensity))) == ConsoleColor::Intensity)
            command += ";9";
        else
            command += ";3";
        command += GetAnsiColorCode(foregroundColor);
    }
    if (backgroundColor != ConsoleColor::Default)
    {
        if ((static_cast<ConsoleColor>(static_cast<int16_t>(backgroundColor) &
                                       static_cast<int16_t>(ConsoleColor::Intensity))) == ConsoleColor::Intensity)
            command += ";10";
        else
            command += ";4";
        command += GetAnsiColorCode(backgroundColor);
    }
    command += "m";
    if (_stream != nullptr)
        *_stream << command;
    _currentForegroundColor = foregroundColor;
    _currentBackgroundColor = backgroundColor;
}

bool Console::ShouldUseColor()
{
    if (_handle == InvalidHandle)
        return false;
    if (isatty(_handle) == 0)
        return false;

    const char * termSetting = getenv("TERM");
    if (termSetting == nullptr)
        return false;
    string term = termSetting;
    const bool term_supports_color =
    (term == "xterm") ||
    (term == "xterm-color") ||
    (term == "xterm-256color") ||
    (term == "screen") ||
    (term == "screen-256color") ||
    (term == "linux") ||
    (term == "cygwin");
    return term_supports_color;
}

static Console defaultConsole;

Console & DefaultConsole() { return defaultConsole; }

} // namespace Tools
