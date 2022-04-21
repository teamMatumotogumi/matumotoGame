#include "PrimitiveValue.h"
#include <cassert>

const PrimitiveValue PrimitiveValue::Empty(TypeCode::Empty);

PrimitiveValue::PrimitiveValue(TypeCode typeCode)
    : m_typeCode(typeCode)
{
    m_sint = 0;
}

PrimitiveValue::PrimitiveValue(const std::string& valueString)
{
    // パース
}


PrimitiveValue PrimitiveValue::ConvertTo(TypeCode type) const
{
    // 同タイプならコピー
    if (m_typeCode == type)
    {
        return *this;
    }

    enum class ValueTypeCategory
    {
        Undefined,
        SInt,
        UInt,
        Real,
    };

    // 変換元タイプを「符号付き整数」「符号なし整数」「それ以外の実数」カテゴリで分ける
    ValueTypeCategory source = ValueTypeCategory::Undefined;
    switch (m_typeCode)
    {
        case TypeCode::Char:
        case TypeCode::Int8:
        case TypeCode::Int16:
        case TypeCode::Int32:
        case TypeCode::Int64:
            source = ValueTypeCategory::SInt;
            break;

        case TypeCode::Boolean:
        case TypeCode::UInt8:
        case TypeCode::UInt16:
        case TypeCode::UInt32:
        case TypeCode::UInt64:
            source = ValueTypeCategory::UInt;
            break;

        case TypeCode::Single:
        case TypeCode::Double:
            source = ValueTypeCategory::Real;
            break;
    }


    // 変換先タイプで分岐
    switch (type)
    {
        case TypeCode::Boolean:
            switch (source) {
                case ValueTypeCategory::SInt: return PrimitiveValue::FromBoolean(m_sint != 0);
                case ValueTypeCategory::UInt: return PrimitiveValue::FromBoolean(m_uint != 0);
                case ValueTypeCategory::Real: return PrimitiveValue::FromBoolean(m_real != 0);
            }
            break;

        case TypeCode::Char:
            switch (source) {
                case ValueTypeCategory::SInt: return PrimitiveValue::FromChar((char)m_sint);
                case ValueTypeCategory::UInt: return PrimitiveValue::FromChar((char)m_uint);
                case ValueTypeCategory::Real: return PrimitiveValue::FromChar((char)m_real);
            }
            break;

        case TypeCode::Int8:
            switch (source) {
                case ValueTypeCategory::SInt: return PrimitiveValue::FromSByte((int8_t)m_sint);
                case ValueTypeCategory::UInt: return PrimitiveValue::FromSByte((int8_t)m_uint);
                case ValueTypeCategory::Real: return PrimitiveValue::FromSByte((int8_t)m_real);
            }
            break;

        case TypeCode::Int16:
            switch (source) {
                case ValueTypeCategory::SInt: return PrimitiveValue::FromInt16((int16_t)m_sint);
                case ValueTypeCategory::UInt: return PrimitiveValue::FromInt16((int16_t)m_uint);
                case ValueTypeCategory::Real: return PrimitiveValue::FromInt16((int16_t)m_real);
            }
            break;

        case TypeCode::Int32:
            switch (source) {
                case ValueTypeCategory::SInt: return PrimitiveValue::FromInt32((int32_t)m_sint);
                case ValueTypeCategory::UInt: return PrimitiveValue::FromInt32((int32_t)m_uint);
                case ValueTypeCategory::Real: return PrimitiveValue::FromInt32((int32_t)m_real);
            }
            break;

        case TypeCode::Int64:
            switch (source) {
                case ValueTypeCategory::SInt: return PrimitiveValue::FromInt64((int64_t)m_sint);
                case ValueTypeCategory::UInt: return PrimitiveValue::FromInt64((int64_t)m_uint);
                case ValueTypeCategory::Real: return PrimitiveValue::FromInt64((int64_t)m_real);
            }
            break;

        case TypeCode::UInt8:
            switch (source) {
                case ValueTypeCategory::SInt: return PrimitiveValue::FromByte((uint8_t)m_sint);
                case ValueTypeCategory::UInt: return PrimitiveValue::FromByte((uint8_t)m_uint);
                case ValueTypeCategory::Real: return PrimitiveValue::FromByte((uint8_t)m_real);
            }
            break;

        case TypeCode::UInt16:
            switch (source) {
                case ValueTypeCategory::SInt: return PrimitiveValue::FromUInt16((uint16_t)m_sint);
                case ValueTypeCategory::UInt: return PrimitiveValue::FromUInt16((uint16_t)m_uint);
                case ValueTypeCategory::Real: return PrimitiveValue::FromUInt16((uint16_t)m_real);
            }
            break;

        case TypeCode::UInt32:
            switch (source) {
                case ValueTypeCategory::SInt: return PrimitiveValue::FromUInt32((uint32_t)m_sint);
                case ValueTypeCategory::UInt: return PrimitiveValue::FromUInt32((uint32_t)m_uint);
                case ValueTypeCategory::Real: return PrimitiveValue::FromUInt32((uint32_t)m_real);
            }
            break;

        case TypeCode::UInt64:
            switch (source) {
                case ValueTypeCategory::SInt: return PrimitiveValue::FromUInt64((uint64_t)m_sint);
                case ValueTypeCategory::UInt: return PrimitiveValue::FromUInt64((uint64_t)m_uint);
                case ValueTypeCategory::Real: return PrimitiveValue::FromUInt64((uint64_t)m_real);
            }
            break;

        case TypeCode::Single:
            switch (source) {
                case ValueTypeCategory::SInt: return PrimitiveValue::FromSingle((float)m_sint);
                case ValueTypeCategory::UInt: return PrimitiveValue::FromSingle((float)m_uint);
                case ValueTypeCategory::Real: return PrimitiveValue::FromSingle((float)m_real);
            }
            break;

        case TypeCode::Double:
            switch (source) {
                case ValueTypeCategory::SInt: return PrimitiveValue::FromDouble((double)m_sint);
                case ValueTypeCategory::UInt: return PrimitiveValue::FromDouble((double)m_uint);
                case ValueTypeCategory::Real: return PrimitiveValue::FromDouble((double)m_real);
            }
            break;
    }

    assert(0);
    return Empty;
}

bool PrimitiveValue::Equals(const PrimitiveValue& other) const
{
    return operator==(*this, other);
}

PrimitiveValue PrimitiveValue::FromString(const std::string& valueString)
{
    assert(0);
    return Empty;
}

bool operator==(const PrimitiveValue& left, const PrimitiveValue& right)
{
    assert(0);
    return false;
}
