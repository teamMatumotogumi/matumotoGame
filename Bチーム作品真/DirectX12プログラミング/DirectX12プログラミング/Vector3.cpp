#include "Vector3.h"

// 静的メンバ変数の宣言
const Vector3 Vector3::Zero(0, 0, 0);
const Vector3 Vector3::One(1, 1, 1);
const Vector3 Vector3::Right(1, 0, 0);
const Vector3 Vector3::Left(-1, 0, 0);
const Vector3 Vector3::Up(0, 1, 0);
const Vector3 Vector3::Down(0, -1, 0);
const Vector3 Vector3::Forward(0, 0, 1);
const Vector3 Vector3::Back(0, 0, -1);
const Vector3 Vector3::NegativeInfinity(Mathf::FloatNegativeInfinity, Mathf::FloatNegativeInfinity, Mathf::FloatNegativeInfinity);
const Vector3 Vector3::PositiveInfinity(Mathf::FloatPositiveInfinity, Mathf::FloatPositiveInfinity, Mathf::FloatPositiveInfinity);


Vector3 operator *(float scalar, const Vector3& vector)
{
    return { scalar * vector.x, scalar * vector.y, scalar * vector.z };
}
