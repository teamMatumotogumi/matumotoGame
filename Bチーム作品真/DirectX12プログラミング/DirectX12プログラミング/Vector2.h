#pragma once

//---------------------------------------------------------------------------------------------------------------------------------------------
// 2次元ベクトルクラス
// 
//      ・2次元空間での位置や向きを表現できる。
//      ・ゲーム開発において非常に重要なもの。
//      ・DirectXMathに含まれるDirectX::XMFLOAT2型は使い難いので
//        ↓のようなラッパークラスを用意しよう。
//---------------------------------------------------------------------------------------------------------------------------------------------
class Vector2
{
public:
    // よく使用するベクトルを定数で定義しておくと便利
    static const Vector2 Zero;              // 全ての成分が0のベクトル (零ベクトル)
    static const Vector2 One;               // 全ての成分が1のベクトル
    static const Vector2 Left;              // 左方向を表すベクトル(-1, 0)
    static const Vector2 Right;             // 右方向を表すベクトル( 1, 0)
    static const Vector2 Down;              // 下方向を表すベクトル( 0,-1)
    static const Vector2 Up;                // 上方向を表すベクトル( 0, 1)
    static const Vector2 NegativeInfinity;  // 全ての成分が負の無限大のベクトル
    static const Vector2 PositiveInfinity;  // 全ての成分が正の無限大のベクトル

public:
    float x;
    float y;

public:
    // デフォルトコンストラクタ
    Vector2();

    // 引数付きコンストラクタ
    Vector2(float x, float y);

    // ベクトルの長さを取得します。
    float Magnitude() const;

    // ベクトルの長さの2乗を取得します。
    float MagnitudeSquare() const;

    // 正規化したベクトルを作成します。
    Vector2 Normalized() const;

    // このベクトルを正規化します。
    void Normalize();

    // このベクトルの成分を設定します。
    void Set(float newX, float newY);

    // このベクトルの各成分に乗算します。
    void Scale(const Vector2& scale);

    // このベクトルを表す文字列を作成します。
    std::string ToString() const;

    // fromとtoの間の正の成す角を取得します。
    static float Angle(const Vector2& from, const Vector2& to);

    // maxLengthの大きさに制限されたvectorのコピーを作成します。
    static Vector2 ClampMagnitude(const Vector2& vector, float maxLength);

    // aとbの距離を返します。
    static float Distance(const Vector2& a, const Vector2& b);

    // aとbの内積を返します。
    static float Dot(const Vector2& lhs, const Vector2& rhs);

    // aとbの間で線形補間します。
    static Vector2 Lerp(const Vector2& a, const Vector2& b, float t);

    // aとbの間で線形補間します。
    static Vector2 LerpUnclamped(const Vector2& a, const Vector2& b, float t);

    // aとbの各成分の一番大きな値を使用してベクトルを作成します
    static Vector2 Max(const Vector2& lhs, const Vector2& rhs);

    // aとbの各成分の一番小さな値を使用してベクトルを作成します
    static Vector2 Min(const Vector2& lhs, const Vector2& rhs);

    // 現在の位置 current から target に向けて移動します。
    static Vector2 MoveTowards(const Vector2& current, const Vector2& target, float maxDistanceDelta);

    // inDirectionを反時計回りに90度回転させたベクトルを作成します。
    static Vector2 Perpendicular(const Vector2& inDirection);

    // 法線を基準にしてベクトルの反射したベクトルを作成します。
    static Vector2 Reflect(const Vector2& inDirection, const Vector2& inNormal);

    // 2つのベクトルの各成分を乗算します
    static Vector2 Scale(const Vector2& a, const Vector2& b);

    // fromとtoの間の成す角を取得します。 [-180°～ +180°]
    // 2つのベクトル間の符号付きの鋭角時計回りの角度です。
    // これは2つのベクトル間の2つの可能な角度のうち小さい方が使用されることを意味します。
    static float SignedAngle(const Vector2& from, const Vector2& to);

    // 目的地に向かって時間の経過とともに徐々にベクトルを変化させます。
    static Vector2 SmoothDamp(const Vector2& current, const Vector2& target, Vector2& currentVelocity, float smoothTime, float maxSpeed = Mathf::FloatPositiveInfinity, float deltaTime = 0.0f);
};



// インライン実装ファイル
#include "Vector2.inl"

