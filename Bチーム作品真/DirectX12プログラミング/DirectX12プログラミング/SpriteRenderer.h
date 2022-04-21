#pragma once
#include "Renderer.h"
#include "Color.h"
#include <DirectXMath.h>

// 前方宣言
class Sprite;
class ConstantBuffer;
class Color;

//---------------------------------------------------------------------------------------------------------------------------------------------
// スプライトレンダラー
//
//   ・スプライト1個をレンダリングするのに必要なリソースや機能を持ったクラス。
//   ・レンダリング時のオプション機能として「スプライトカラー」「上下反転」「左右反転」等が指定できる。
// 
//---------------------------------------------------------------------------------------------------------------------------------------------
class SpriteRenderer : public Renderer
{
private:
    Sprite* m_sprite;                   // レンダリング対象となるスプライト
    Color m_spriteColor;                // スプライトカラー
    bool m_isFlippedX;                  // テクスチャを左右反転したい場合は true
    bool m_isFlippedY;                  // テクスチャを上下反転したい場合は true
    ConstantBuffer* m_constantBuffer;   // 定数バッファ
    struct ConstantBufferLayout;        // 定数バッファレイアウト
    friend class GameObject;            // ゲームオブジェクトクラスは友達

private:
    // コンストラクタ
    SpriteRenderer();

    // 仮想デストラクタ
    virtual ~SpriteRenderer();

    // Component::Render()のオーバーライド
    void Render() override;

public:
    // このデータ型の情報を返します。
    static const TypeInfo& GetTypeInfo();

    // このインスタンスのデータ型の情報を返します。
    virtual const TypeInfo& GetTypeInfoOfInstance() const { return GetTypeInfo(); }

    // レンダリング対象となるスプライトを設定します。
    void SetSprite(Sprite* sprite);

    // レンダリング対象となるスプライトを取得します。
    Sprite* GetSprite() const { return m_sprite; }

    // スプライトカラーを設定します。
    void SetColor(const Color& color);

    // スプライトカラーを取得します。
    const Color& GetColor() const { return m_spriteColor; }
};
