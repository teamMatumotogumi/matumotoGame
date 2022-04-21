#include "Vector2.h"


inline Vector2::Vector2()
{
    // 何もしない
}


inline Vector2::Vector2(float x, float y)
    : x(x)
    , y(y)
{

}


// 演算子のオーバーロード
inline Vector2 operator + (const Vector2& a);
inline Vector2 operator - (const Vector2& a);
inline Vector2 operator + (const Vector2& a, const Vector2& b);
inline Vector2 operator - (const Vector2& a, const Vector2& b);
inline Vector2 operator * (const Vector2& a, float scalar);
inline Vector2 operator * (float scalar, const Vector2& a);
inline Vector2 operator * (const Vector2& a, const Vector2& b);
inline Vector2 operator / (const Vector2& a, float scalar);
inline Vector2 operator / (const Vector2& a, const Vector2& b);
