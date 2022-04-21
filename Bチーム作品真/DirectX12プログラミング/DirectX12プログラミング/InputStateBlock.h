#pragma once
#include <cstdint>
#include "FourCC.h"

// 前方宣言
class PrimitiveValue;

struct InputStateBlock
{
private:
    FourCC m_format;
    uint32_t m_sizeInBits;
    uint32_t m_bitOffset;
    uint32_t m_byteOffset;

public:
    const uint32_t AutomaticOffset = 4294967294U;
    const uint32_t InvalidOffset = 4294967295U;
    static const FourCC FormatBit;
    static const FourCC FormatByte;
    static const FourCC FormatDouble;
    static const FourCC FormatFloat;
    static const FourCC FormatInt;
    static const FourCC FormatLong;
    static const FourCC FormatQuaternion;
    static const FourCC FormatSBit;
    static const FourCC FormatSByte;
    static const FourCC FormatShort;
    static const FourCC FormatUInt;
    static const FourCC FormatULong;
    static const FourCC FormatUShort;
    static const FourCC FormatVector2;
    static const FourCC FormatVector2Byte;
    static const FourCC FormatVector2Short;
    static const FourCC FormatVector3;
    static const FourCC FormatVector3Byte;
    static const FourCC FormatVector3Short;

public:
    uint32_t GetBitOffset() const { return m_bitOffset; }
    void SetBitOffset(uint32_t value) { m_bitOffset = value; }

    uint32_t GetByteOffset() const { return m_byteOffset; }
    void SetByteOffset(uint32_t value) { m_byteOffset = value; }
        
    const FourCC& GetFormat() const { return m_format; }
    void SetFormat(const FourCC& fourCC) { m_format = fourCC; }

    uint32_t GetSizeInBits() const { return m_sizeInBits; }
    void SetSizeInBits(uint32_t sizeInBits) { m_sizeInBits = sizeInBits; }

    void CopyToFrom(void *toStatePtr, void *fromStatePtr);

//  static FourCC GetPrimitiveFormatFromType(Type type);
    static int32_t GetSizeOfPrimitiveFormatInBits(FourCC type);

    double ReadDouble(void *statePtr);
    float ReadFloat(void *statePtr);
    int ReadInt(void *statePtr);

    void Write(void *statePtr, const PrimitiveValue& value);
    void WriteDouble(void *statePtr, double value);
    void WriteFloat(void *statePtr, float value);
    void WriteInt(void *statePtr, int value);
};

