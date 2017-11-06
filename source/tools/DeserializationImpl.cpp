#include "tools/DeserializationImpl.hpp"

#include <climits>
#include <cfloat>
#include <ctgmath>
#include <cstring>
#include "tools/Util.hpp"

using namespace std;

static bool HasValidCharactersForBase(const string & text, int base, bool maybeSigned)
{
    if (text.empty())
        return false;
    switch (base)
    {
        case 2:
            for (size_t i = 0; i < text.length(); i++)
            {
                if (strchr("01", static_cast<char>(toupper(text[i]))) == nullptr)
                    return false;
            }
            break;
        case 8:
            for (size_t i = 0; i < text.length(); i++)
            {
                if (strchr("01234567", static_cast<char>(toupper(text[i]))) == nullptr)
                    return false;
            }
            break;
        case 10:
            for (size_t i = 0; i < text.length(); i++)
            {
                if (maybeSigned)
                {
                    if (strchr("0123456789+-",
                                              static_cast<char>(toupper(text[i]))) == nullptr)
                        return false;
                }
                else
                {
                    if (strchr("0123456789+",
                                              static_cast<char>(toupper(text[i]))) == nullptr)
                        return false;
                }
            }
            break;
        case 16:
            for (size_t i = 0; i < text.length(); i++)
            {
                if (strchr("0123456789ABCDEF", static_cast<char>(toupper(text[i]))) == nullptr)
                    return false;
            }
            break;
        default:
            return false;
    }
    return true;
}

static bool HasValidCharactersFloatingPoint(const string & text)
{
    if (text.empty())
        return false;
    for (size_t i = 0; i < text.length(); i++)
    {
        if (strchr("0123456789.+-Ee", static_cast<char>(toupper(text[i]))) == nullptr)
            return false;
    }
    return true;
}


bool Tools::Deserialize(const string & text, bool & value)
{
    if (IsEqualIgnoreCase("true", text))
    {
        value = true;
        return true;
    }
    if (IsEqualIgnoreCase("false", text))
    {
        value = false;
        return true;
    }

    return false;
}

bool Tools::Deserialize(const string & text, int8_t & value, int base)
{
    value = {};
    if (!HasValidCharactersForBase(text, base, true))
        return false;

    long result = strtol(text.c_str(), nullptr, base);
    if ((base == 10) && ((result < CHAR_MIN) || (result > CHAR_MAX)))
        return false;
    value = static_cast<int8_t>(result);

    return true;
}

bool Tools::Deserialize(const string & text, uint8_t & value, int base /*= 10*/)
{
    value = {};
    if (!HasValidCharactersForBase(text, base, false))
        return false;

    long result = strtol(text.c_str(), nullptr, base);
    if ((base == 10) && ((result < 0) || (result > UCHAR_MAX)))
        return false;
    value = static_cast<uint8_t>(result);

    return true;
}

bool Tools::Deserialize(const string & text, int16_t & value, int base /*= 10*/)
{
    value = {};
    if (!HasValidCharactersForBase(text, base, true))
        return false;

    long result = strtol(text.c_str(), nullptr, base);
    if ((base == 10) && ((result < SHRT_MIN) || (result > SHRT_MAX)))
        return false;
    value = static_cast<int16_t>(result);

    return true;
}

bool Tools::Deserialize(const string & text, uint16_t & value, int base /*= 10*/)
{
    value = {};
    if (!HasValidCharactersForBase(text, base, false))
        return false;

    long result = strtol(text.c_str(), nullptr, base);
    if ((base == 10) && ((result < 0) || (result > USHRT_MAX)))
        return false;
    value = static_cast<uint16_t>(result);

    return true;
}

bool Tools::Deserialize(const string & text, int32_t & value, int base /*= 10*/)
{
    value = {};
    if (!HasValidCharactersForBase(text, base, true))
        return false;

    long result = strtol(text.c_str(), nullptr, base);
    unsigned long result_ul = strtoul(text.c_str(), nullptr, base);
    if (result < INT_MIN)
        return false;
    if ((base == 10) && (result >= 0) && (result_ul > INT_MAX))
        return false;
    if ((base != 10) && (result_ul > INT_MAX))
        value = static_cast<int32_t>(result_ul);
    else
        value = static_cast<int32_t>(result);

    return true;
}

bool Tools::Deserialize(const string & text, uint32_t & value, int base /*= 10*/)
{
    value = {};
    if (!HasValidCharactersForBase(text, base, false))
        return false;

    unsigned long result = strtoul(text.c_str(), nullptr, base);
    if ((base == 10) && (result > UINT_MAX))
        return false;
    value = static_cast<uint32_t>(result);

    return true;
}

bool Tools::Deserialize(const string & text, int64_t & value, int base /*= 10*/)
{
    value = {};
    if (!HasValidCharactersForBase(text, base, true))
        return false;

    // For some strange reason stroull behaves incorrectly in some cases.
    long long result = strtoll(text.c_str(), nullptr, base);
    unsigned long long result_ull = strtoull(text.c_str(), nullptr, base);
    if (result < LLONG_MIN)
        return false;
    if ((base == 10) && (result >= 0) && (result_ull > LLONG_MAX))
        return false;
    if ((base != 10) && (result_ull > LLONG_MAX))
        value = static_cast<int64_t>(result_ull);
    else
        value = result;

    return true;
}

bool Tools::Deserialize(const string & text, uint64_t & value, int base /*= 10*/)
{
    value = {};
    if (!HasValidCharactersForBase(text, base, false))
        return false;

    unsigned long long result = strtoull(text.c_str(), nullptr, base);
    value = static_cast<uint64_t>(result);

    return true;
}

bool Tools::Deserialize(const string & text, float & value)
{
    value = {};
    if (!HasValidCharactersFloatingPoint(text))
        return false;

    long double tmp;
    istringstream stream(text);
    stream >> tmp;
    if ((abs(tmp) < FLT_MIN) || (abs(tmp) > FLT_MAX))
        return false;

    value = static_cast<float>(tmp);
    return true;
}

bool Tools::Deserialize(const string & text, double & value)
{
    value = {};
    if (!HasValidCharactersFloatingPoint(text))
        return false;

    long double tmp;
    istringstream stream(text);
    stream >> tmp;
    if ((abs(tmp) < DBL_MIN) || (abs(tmp) > DBL_MAX))
        return false;

    value = static_cast<double>(tmp);
    return true;
}

bool Tools::Deserialize(const string & text, long double & value)
{
    value = {};
    if (!HasValidCharactersFloatingPoint(text))
        return false;

    istringstream stream(text);
    stream >> value;

    return true;
}

