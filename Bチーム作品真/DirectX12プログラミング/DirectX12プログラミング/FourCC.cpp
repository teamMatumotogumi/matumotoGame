#include "FourCC.h"

FourCC::FourCC(char a, char b, char c, char d) noexcept
{
    m_code = a | (b << 8) | (c << 16) | (d << 24); 
}

FourCC::FourCC(int32_t code) noexcept
    : m_code(code)
{
}

FourCC::FourCC(const char code[4]) noexcept
{
    m_code = code[0] | (code[1] << 8) | (code[2] << 16) | (code[3] << 24); 
}

bool FourCC::Equals(const FourCC& other) const noexcept
{
    return (m_code == other.m_code);
}

FourCC FourCC::FromInt32(int32_t i) noexcept
{
    return i;
}

int32_t FourCC::ToInt32(const FourCC& fourCC) noexcept
{
    return fourCC.m_code;
}

FourCC::operator int() const noexcept
{
    return m_code;
}

bool operator==(const FourCC& left, const FourCC& right) noexcept
{
    return (left.m_code == right.m_code);
}

bool operator!=(const FourCC& left, const FourCC& right) noexcept
{
    return (left.m_code != right.m_code);
}
