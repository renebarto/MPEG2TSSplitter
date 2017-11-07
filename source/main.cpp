#include <iostream>
#include <iomanip>
#include "tools/Console.hpp"
#include "media/TSDefinitions.hpp"
#include "media/MPEG2Splitter.hpp"
#include "CommandLineOptionsParser.hpp"

using namespace std;

int main(int argc, const char * argv[])
{
    CommandLineOptionsParser parser(Tools::DefaultConsole());

    if (!parser.Parse(argc, argv))
        return EXIT_FAILURE;
    string path = parser._inputPath;
    if (path.empty())
    {
        Tools::DefaultConsole() << Tools::fgcolor(Tools::ConsoleColor::Red) << "No input path specified"
                                << Tools::fgcolor(Tools::ConsoleColor::Default) << endl;
        Tools::DefaultConsole() << parser.GetHelp("MPEG2TS-splitter");
        return EXIT_FAILURE;
    }
    Tools::DefaultConsole() << Tools::fgcolor(Tools::ConsoleColor::Yellow) << "Splitting file: " << path
                            << Tools::fgcolor(Tools::ConsoleColor::Default) << endl;
    Media::MPEG2Splitter splitter(path);
    if (parser._audioPID != static_cast<uint16_t>(Media::PIDType::NULL_PACKET))
    {
        Tools::DefaultConsole() << Tools::fgcolor(Tools::ConsoleColor::Green) << "Using predefined audio PID: "
                                << hex << setw(4) << setfill('0') << parser._audioPID << dec
                                << Tools::fgcolor(Tools::ConsoleColor::Default) << endl;
        splitter.SetAudioPID(static_cast<Media::PIDType>(parser._audioPID));
    } else
    {
        Tools::DefaultConsole() << Tools::fgcolor(Tools::ConsoleColor::Green) << "Using first audio PID found"
                                << Tools::fgcolor(Tools::ConsoleColor::Default) << endl;
    }
    if (parser._videoPID != static_cast<uint16_t>(Media::PIDType::NULL_PACKET))
    {
        Tools::DefaultConsole() << Tools::fgcolor(Tools::ConsoleColor::Green) << "Using predefined video PID: "
                                << hex << setw(4) << setfill('0') << parser._videoPID << dec
                                << Tools::fgcolor(Tools::ConsoleColor::Default) << endl;
        splitter.SetVideoPID(static_cast<Media::PIDType>(parser._videoPID));
    } else
    {
        Tools::DefaultConsole() << Tools::fgcolor(Tools::ConsoleColor::Green) << "Using first video PID found"
                                << Tools::fgcolor(Tools::ConsoleColor::Default) << endl;
    }
    splitter.SetVerbose(parser._verbose);
    return splitter.Run();
}