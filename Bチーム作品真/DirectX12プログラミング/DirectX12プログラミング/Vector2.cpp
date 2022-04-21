#include "Vector2.h"

// 静的メンバ変数の宣言
const Vector2 Vector2::Zero(0, 0);
const Vector2 Vector2::One(1, 1);
const Vector2 Vector2::Left(-1, 0);
const Vector2 Vector2::Right(1, 0);
const Vector2 Vector2::Down(0, -1);
const Vector2 Vector2::Up(0, 1);
const Vector2 Vector2::NegativeInfinity(Mathf::FloatNegativeInfinity, Mathf::FloatNegativeInfinity);
const Vector2 Vector2::PositiveInfinity(Mathf::FloatPositiveInfinity, Mathf::FloatPositiveInfinity);


// 非インラインメンバ関数の実装


