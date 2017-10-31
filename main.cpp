#include <iostream>
#include "MPEG2Demultiplexor.hpp"

using namespace std;

string StripPath(const string & path)
{
    size_t lastSlashPos = path.find_last_of('/');
    if (lastSlashPos != string::npos)
        return path.substr(lastSlashPos + 1);
}

int Usage(const string & applicationPath, bool error)
{
    string applicationName = StripPath(applicationPath);

    cout << "Usage " << applicationName << " [-h] [file]" << endl << endl
         << "If -h is specified, prints this help." << endl << endl
         << "One file must be specified. This file must be a valid MPEG2 TS stream, which will be " << endl
         << "de-multiplexed into an elementary video and audio stream. Only the first audio and video" << endl
         << "stream will be extracted and saved into files." << endl
         << "File extensions will be chosen such that the stream can be played in a media player, such as VLC.";
    return (error) ? EXIT_FAILURE : EXIT_SUCCESS;
}

bool EqualIgnoreCase(const string &a , const string & b)
{
    return std::equal(a.begin(), a.end(), b.begin(),
                      [](const char & a, const char & b)
                      {
                          return (std::tolower(a) == std::tolower(b));
                      });
}

int main(int argc, char * argv[])
{
    if ((argc < 2) || (argc > 2))
        return Usage(argv[0], true);
    if (EqualIgnoreCase(argv[1], "-h"))
        return Usage(argv[0], false);
    Media::MPEG2Demultiplexor demux(argv[1]);
    return demux.Run();
}