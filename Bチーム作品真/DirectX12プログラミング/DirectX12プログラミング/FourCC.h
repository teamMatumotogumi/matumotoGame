#pragma once
#include <cstdint>

class FourCC
{
private:
    int32_t m_code;

public:
    __declspec(deprecated) FourCC() {}

    // 4文字から4文字コードを生成します。
    FourCC(char a, char b, char c, char d) noexcept;

    // 4文字から4文字コードを生成します。
    FourCC(const char code[4]) noexcept;

    // 32ビット整数値から4文字コードを生成します。
    FourCC(int32_t code) noexcept;

    // 4文字コード同士を比較し、等価の場合は true を返します。
    bool Equals(const FourCC& other) const noexcept;

    // 32ビット整数値から4文字コードを生成します。
    static FourCC FromInt32(int32_t i) noexcept;

    // 4文字コードから32整数値を生成します。
    static int32_t ToInt32(const FourCC& fourCC) noexcept;

    // 4文字コードを表す32整数値を取得します。
    operator int() const noexcept;

    // 4文字コード同士を比較し、等価の場合は true を返します。
    friend bool operator ==(const FourCC& left, const FourCC& right) noexcept;

    // 4文字コード同士を比較し、不等価の場合は true を返します。
    friend bool operator !=(const FourCC& left, const FourCC& right) noexcept;
};


// 4文字コード同士を比較し、等価の場合は true を返します。
bool operator ==(const FourCC& left, const FourCC& right) noexcept;

// 4文字コード同士を比較し、不等価の場合は true を返します。
bool operator !=(const FourCC& left, const FourCC& right) noexcept;

