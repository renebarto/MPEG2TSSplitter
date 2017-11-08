#include "media/StreamWriter.hpp"

#include <iostream>

using namespace Media;

void StreamWriter::FeedData(Tools::ByteIterator data, size_t size)
{
    _stream.write(reinterpret_cast<const char *>(&(*data)), size);
}
