#pragma once
#include <DirectXMath.h>

//---------------------------------------------------------------------------------------------------------------------------------------------
// カラークラス
// 
//      ・「光の三原色(RGB)」を用いたコンピューター内部での色の表現方法。
//      ・「R値」「G値」「B値」に「α値」を加えた4コンポーネントで表される。
//      ・ゲーム開発において色の扱いは重要。
//      ・DirectXMathに含まれるDirectX::XMFLOAT4型は使い難いので
//        ↓のようなラッパークラスを用意しよう。
//---------------------------------------------------------------------------------------------------------------------------------------------
class Color
{
public:
    // よく知られた色を定数で定義しておくと便利
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color White;
    static const Color Gray;
    static const Color Black;
    static const Color TransparentBlack;
    static const Color CornflowerBlue;
    static const Color Gold;

public:
    union
    {
        struct { float r, g, b, a; };
        float components[4];
    };

public:
    // コンストラクタ
    Color();

    // コンストラクタ
    Color(float r, float g, float b, float a);
};
