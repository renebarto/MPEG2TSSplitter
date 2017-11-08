#include "Util.hpp"

#include <cstddef>
#include <climits>
#include <cctype>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstring>
#include "tools/Console.hpp"
#include "SerializationImpl.hpp"

using namespace std;
namespace Tools
{

void DumpBytes(ByteIterator begin, ByteIterator end)
{
    constexpr size_t BytesPerColumn = 16;
    size_t offset = 0;
    size_t size = end - begin;
    ByteIterator current = begin;
    while (offset < size)
    {
        for (size_t i = 0; i < BytesPerColumn; ++i)
        {
            if ((offset + i) < size)
                Tools::DefaultConsole() << Tools::Serialize(current[i], 16) << " ";
            else
                Tools::DefaultConsole() << "   ";
        }
        for (size_t i = 0; i < BytesPerColumn; ++i)
        {
            if ((offset + i) < size)
                Tools::DefaultConsole() << (isprint(current[i]) ? (char)current[i] : '.') << " ";
            else
                Tools::DefaultConsole() << "  ";
        }
        offset += BytesPerColumn;
        current += BytesPerColumn;
        Tools::DefaultConsole() << endl;
    }
}

std::string Trim(const std::string & value, const std::string & stripChars)
{
    const char * find = stripChars.c_str();

    size_t indexLeft = 0;
    while ((indexLeft < value.length()) && strchr(find, value[indexLeft]))
        ++indexLeft;
    size_t indexRight = value.length();
    while ((indexRight > indexLeft) && strchr(find, value[indexRight - 1]))
        --indexRight;
    return value.substr(indexLeft, indexRight - indexLeft);
}

static const char _PathSeparator = '/';

char PathSeparator()
{
    return _PathSeparator;
}

std::string LastPartOfPath(const std::string & path)
{
    size_t lastPathDelimiterPos = path.find_last_of(PathSeparator());
    if (lastPathDelimiterPos != std::string::npos)
    {
        return path.substr(lastPathDelimiterPos + 1);
    }
    return path;
}

bool IsEqual(const string & lhs, const string & rhs)
{
    return (lhs == rhs);
}

bool IsEqualIgnoreCase(const string & lhs, const string & rhs)
{
    if (lhs.size() != rhs.size())
        return false;

    size_t size = rhs.size();
    for (size_t n = 0; n < size; n++)
        if (tolower(lhs[n]) != tolower(rhs[n]))
            return false;

    return true;
}

bool IsEqual(const char * lhs, const char * rhs)
{
    return strcmp(lhs, rhs) == 0;
}

bool IsEqualIgnoreCase(const char * lhs, const char * rhs)
{
    return strcasecmp(lhs, rhs) == 0;
}

void ThrowOnError(const char * functionName, const char * fileName, int line, int errorCode)
{
    if (errorCode != 0)
    {
        ostringstream stream;
        if (functionName != nullptr)
            stream << "in " << functionName;
        if (fileName != nullptr)
        {
            stream << " [" << fileName;
            if (line > 0)
                stream << ":" << line;
            stream << "]";
        }
        stream << " errno=" << errorCode << " (0x" << Serialize(errorCode, 16) << "): " << '"' << strerror(errorCode) << '"';
        throw runtime_error(stream.str());
    }
}
} // namespace Tools
