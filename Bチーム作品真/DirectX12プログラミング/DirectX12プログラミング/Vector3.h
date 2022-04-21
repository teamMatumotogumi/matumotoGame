#pragma once

//---------------------------------------------------------------------------------------------------------------------------------------------
// 3次元ベクトルクラス
// 
//      ・3次元空間での位置や向きを表現できる。
//      ・ゲーム開発において非常に重要なもの。
//      ・DirectXMathに含まれるDirectX::XMFLOAT3型は使い難いので
//        ↓のようなラッパークラスを用意しよう。
//---------------------------------------------------------------------------------------------------------------------------------------------
class Vector3
{
public:
    // よく使用するベクトルを定数で定義しておくと便利
    static const Vector3 Zero;              // 全ての成分が0のベクトル (零ベクトル)
    static const Vector3 One;               // 全ての成分が1のベクトル
    static const Vector3 Right;             // 右方向を表す単位ベクトル
    static const Vector3 Left;              // 左方向を表す単位ベクトル
    static const Vector3 Up;                // 上方向を表す単位ベクトル   
    static const Vector3 Down;              // 下方向を表す単位ベクトル
    static const Vector3 Forward;           // 前方向を表す単位ベクトル
    static const Vector3 Back;              // 後方向を表す単位ベクトル
    static const Vector3 NegativeInfinity;  // 全ての成分が負の無限大のベクトル
    static const Vector3 PositiveInfinity;  // 全ての成分が正の無限大のベクトル

public:
    float x;
    float y;
    float z;

public:
    // デフォルトコンストラクタ
    Vector3();

    // 引数付きコンストラクタ
    Vector3(float x, float y, float z);
};


// インライン実装ファイル
#include "Vector3.inl"


// 演算子のオーバーロード
Vector3 operator *(float scalar, const Vector3& vector);
