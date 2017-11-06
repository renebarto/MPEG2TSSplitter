#pragma once

#include "tools/Serialization.hpp"

namespace Tools {

bool Deserialize(const std::string & text, bool & value);
bool Deserialize(const std::string & text, int8_t & value, int base = 10);
bool Deserialize(const std::string & text, uint8_t & value, int base = 10);
bool Deserialize(const std::string & text, int16_t & value, int base = 10);
bool Deserialize(const std::string & text, uint16_t & value, int base = 10);
bool Deserialize(const std::string & text, int32_t & value, int base = 10);
bool Deserialize(const std::string & text, uint32_t & value, int base = 10);
bool Deserialize(const std::string & text, int64_t & value, int base = 10);
bool Deserialize(const std::string & text, uint64_t & value, int base = 10);
bool Deserialize(const std::string & text, float & value);
bool Deserialize(const std::string & text, double & value);
bool Deserialize(const std::string & text, long double & value);
inline bool Deserialize(const std::string & text, std::string & value)
{
    value = text;
    return true;
}

template<class T>
class StringDeserializer : public IStringDeserializer<T>
{
public:
    virtual bool Deserialize(const std::string &text, T &value) override
    {
        T parsedValue;
        if (Tools::Deserialize(text, parsedValue)) {
            value = parsedValue;
            return true;
        }
        value = T{};
        return false;
    }
};

} // namespace Tools
