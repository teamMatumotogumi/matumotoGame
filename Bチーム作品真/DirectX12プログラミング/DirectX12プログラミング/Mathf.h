#pragma once
#include <DirectXMath.h>

class Mathf
{
public:
    // 単精度浮動小数点数(float)の最小値
    static constexpr float FloatNegativeInfinity = -FLT_MAX;

    // 単精度浮動小数点数(float)の最大値
    static constexpr float FloatPositiveInfinity = FLT_MAX;

    // 行列を転置します。
    static void Transpose(DirectX::XMFLOAT4X4& matrix);

    // 行列を転置します。
    static void Transpose(DirectX::XMFLOAT4X4& destination, const DirectX::XMFLOAT4X4& source);
};


