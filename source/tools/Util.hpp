#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace Tools
{

void DumpBytes(const std::vector<uint8_t> & data);

std::string Trim(const std::string & value, const std::string & stripChars);

std::string LastPartOfPath(const std::string & path);

bool IsEqual(const std::string & lhs, const std::string & rhs);
bool IsEqualIgnoreCase(const std::string & lhs, const std::string & rhs);
bool IsEqual(const char * lhs, const char * rhs);
bool IsEqualIgnoreCase(const char * lhs, const char * rhs);

void ThrowOnError(const char * functionName, const char * fileName, int line, int errorCode);

} // namespace Tools
