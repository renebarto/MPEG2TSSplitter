#include <iostream>
#include <iomanip>
#include "tools/Console.hpp"
#include "media/Logging.hpp"
#include "media/TSDefinitions.hpp"
#include "media/MPEG2Splitter.hpp"
#include "CommandLineOptionsParser.hpp"

using namespace std;
using namespace Tools;
using namespace Media;

int main(int argc, const char * argv[])
{
    CommandLineOptionsParser parser(DefaultConsole());

    if (!parser.Parse(argc, argv))
        return EXIT_FAILURE;
    string path = parser._inputPath;
    if (path.empty())
    {
        DefaultConsole() << fgcolor(ConsoleColor::Red) << "No input path specified"
                         << fgcolor(ConsoleColor::Default) << endl;
        DefaultConsole() << parser.GetHelp("MPEG2TS-splitter");
        return EXIT_FAILURE;
    }
    DefaultConsole() << fgcolor(ConsoleColor::Yellow) << "Splitting file: " << path
                     << fgcolor(ConsoleColor::Default) << endl;
    
    VerboseLogging(parser._verbose);
    
    MPEG2Splitter splitter(path);
    if (parser._audioPID != static_cast<uint16_t>(PIDType::NULL_PACKET))
    {
        DefaultConsole() << fgcolor(ConsoleColor::Green) << "Using predefined audio PID: "
                         << hex << setw(4) << setfill('0') << parser._audioPID << dec
                         << fgcolor(ConsoleColor::Default) << endl;
        splitter.SetAudioPID(static_cast<PIDType>(parser._audioPID));
    } else
    {
        DefaultConsole() << fgcolor(ConsoleColor::Green) << "Using first audio PID found"
                         << fgcolor(ConsoleColor::Default) << endl;
    }
    if (parser._videoPID != static_cast<uint16_t>(PIDType::NULL_PACKET))
    {
        DefaultConsole() << fgcolor(ConsoleColor::Green) << "Using predefined video PID: "
                         << hex << setw(4) << setfill('0') << parser._videoPID << dec
                         << fgcolor(ConsoleColor::Default) << endl;
        splitter.SetVideoPID(static_cast<PIDType>(parser._videoPID));
    } else
    {
        DefaultConsole() << fgcolor(ConsoleColor::Green) << "Using first video PID found"
                         << fgcolor(ConsoleColor::Default) << endl;
    }
    return splitter.Run();
}