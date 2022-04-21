#include "Vector4.h"

// 静的メンバ変数の宣言
const Vector4 Vector4::Zero(0, 0, 0, 0);
const Vector4 Vector4::One(1, 1, 1, 1);
const Vector4 Vector4::NegativeInfinity(Mathf::FloatNegativeInfinity, Mathf::FloatNegativeInfinity, Mathf::FloatNegativeInfinity, Mathf::FloatNegativeInfinity);
const Vector4 Vector4::PositiveInfinity(Mathf::FloatPositiveInfinity, Mathf::FloatPositiveInfinity, Mathf::FloatPositiveInfinity, Mathf::FloatPositiveInfinity);


Vector4 operator*(float scalar, const Vector4& vector)
{
    return Vector4();
}
