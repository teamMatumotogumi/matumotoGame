#pragma once
#include <cstdint>
#include <string>

enum class TypeCode
{
    Empty = 0,
    Object = 1,
    DBNull = 2,
    Boolean = 3,
    Char = 4,
    Int8 = 5,
    UInt8 = 6,
    Int16 = 7,
    UInt16 = 8,
    Int32 = 9,
    UInt32 = 10,
    Int64 = 11,
    UInt64 = 12,
    Single = 13,
    Double = 14,
    Decimal = 15,
    DateTime = 16,
    String = 18,
};


class PrimitiveValue
{
private:
    static const PrimitiveValue Empty;

private:
    TypeCode m_typeCode;
    union
    {
        int64_t     m_sint;
        uint64_t    m_uint;
        double      m_real;
    };

private:
    PrimitiveValue(TypeCode typeCode);

public:
    PrimitiveValue(bool value)     : m_typeCode(TypeCode::Boolean), m_uint(value) { }
    PrimitiveValue(char value)     : m_typeCode(TypeCode::Char), m_sint(value) { }
    PrimitiveValue(int8_t value)   : m_typeCode(TypeCode::Int8), m_sint(value) { }
    PrimitiveValue(int16_t value)  : m_typeCode(TypeCode::Int16), m_sint(value) { }
    PrimitiveValue(int32_t value)  : m_typeCode(TypeCode::Int32), m_sint(value) { }
    PrimitiveValue(int64_t value)  : m_typeCode(TypeCode::Int64), m_sint(value) { }
    PrimitiveValue(uint8_t value)  : m_typeCode(TypeCode::UInt8), m_uint(value) { }
    PrimitiveValue(uint16_t value) : m_typeCode(TypeCode::UInt16), m_uint(value) { }
    PrimitiveValue(uint32_t value) : m_typeCode(TypeCode::UInt32), m_uint(value) { }
    PrimitiveValue(uint64_t value) : m_typeCode(TypeCode::UInt64), m_uint(value) { }
    PrimitiveValue(float value)    : m_typeCode(TypeCode::Single), m_real(value) { }
    PrimitiveValue(double value)   : m_typeCode(TypeCode::Double), m_real(value) { }
    PrimitiveValue(const std::string& valueString);

    bool IsEmpty() const { return (m_typeCode == TypeCode::Empty); }
    TypeCode GetTypeCode() const { return m_typeCode; }

    PrimitiveValue ConvertTo(TypeCode type) const;
    bool Equals(const PrimitiveValue& other) const;

    template<typename TValue>
    static PrimitiveValue From(TValue value) { return value; }
    static PrimitiveValue FromBoolean(bool value) { return value; }
    static PrimitiveValue FromChar(char value) { return value; }
    static PrimitiveValue FromSByte(int8_t value) { return value; }
    static PrimitiveValue FromInt16(int16_t value) { return value; }
    static PrimitiveValue FromInt32(int32_t value) { return value; }
    static PrimitiveValue FromInt64(int64_t value) { return value; }
    static PrimitiveValue FromByte(uint8_t value) { return value; }
    static PrimitiveValue FromUInt16(uint16_t value) { return value; }
    static PrimitiveValue FromUInt32(uint32_t value) { return value; }
    static PrimitiveValue FromUInt64(uint64_t value) { return value; }
    static PrimitiveValue FromSingle(float value) { return value; }
    static PrimitiveValue FromDouble(double value) { return value; }
    static PrimitiveValue FromString(const std::string& valueString);
};


bool operator ==(const PrimitiveValue& left, const PrimitiveValue& right);


