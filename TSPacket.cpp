#include "TSPacket.hpp"

namespace Media
{

static uint8_t SyncByte = 0x47;

TSPacket::TSPacket()
{
}

bool TSPacket::IsValid() const
{
    return _data[0] == SyncByte;
}

} // namespace Media
