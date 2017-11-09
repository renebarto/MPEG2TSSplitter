#pragma once

#include <cstdint>
#include <vector>

#define PARAMETER_NOT_USED __attribute__((unused))

using ByteIterator = std::vector<uint8_t>::const_iterator;
