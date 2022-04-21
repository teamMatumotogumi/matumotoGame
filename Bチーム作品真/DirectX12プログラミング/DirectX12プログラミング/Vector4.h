#pragma once

//---------------------------------------------------------------------------------------------------------------------------------------------
// 4次元ベクトルクラス
// 
//      ・4x4型行列との演算に利用される。
//      ・DirectXMathに含まれるDirectX::XMFLOAT4型は使い難いので
//        ↓のようなラッパークラスを用意しよう。
//---------------------------------------------------------------------------------------------------------------------------------------------
class Vector4
{
public:
    // よく使用するベクトルを定数で定義しておくと便利
    static const Vector4 Zero;              // 全ての成分が0のベクトル (零ベクトル)
    static const Vector4 One;               // 全ての成分が1のベクトル
    static const Vector4 NegativeInfinity;  // 全ての成分が負の無限大のベクトル
    static const Vector4 PositiveInfinity;  // 全ての成分が正の無限大のベクトル

public:
    float x;
    float y;
    float z;
    float w;

public:
    // デフォルトコンストラクタ
    Vector4();

    // 引数付きコンストラクタ
    Vector4(float x, float y, float z, float w);
};


// インライン実装ファイル
#include "Vector4.inl"


// 演算子のオーバーロード
Vector4 operator *(float scalar, const Vector4& vector);










