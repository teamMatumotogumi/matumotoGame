#pragma once

//---------------------------------------------------------------------------------------------------------------------------------------------
// ブレンド因子
// 
// ブレンドソース = (Rs, Gs, Bs, As)
// ブレンドデスティネーション = (Rd, Gd, Bd, Ad)
// 
//---------------------------------------------------------------------------------------------------------------------------------------------
enum class BlendFactor
{
    Zero,                       // (   0,    0,    0,    0)
    One,                        // (   1,    1,    1,    1)
    SourceColor,                // (  Rs,   Gs,   Bs,   As)
    SourceAlpha,                // (  As,   As,   As,   As)
    SourceAlphaSaturate,        // (  As,   As,   As,   As) and saturate()
    Source1Color,               // (  R ,   G ,   B ,   A ) by PixelShader()
    Source1Alpha,               // (  A ,   A ,   A ,   A ) by PixelShader()
    BlendFactor,                // (  R ,   G ,   B ,   A ) by OMSetBlendFactor()
    DestinationColor,           // (  Rd,   Gd,   Bd,   Ad)
    DestinationAlpha,           // (  Ad,   Ad,   Ad,   Ad)
    InverseSourceColor,         // (1-Rs, 1-Gs, 1-Bs, 1-As)
    InverseSourceAlpha,         // (1-As, 1-As, 1-As, 1-As)
    InverseSource1Color,        // (1-R,  1-G,  1-B,  1-A ) by PixelShader()
    InverseSource1Alpha,        // (1-A,  1-A,  1-A,  1-A ) by PixelShader()
    InverseBlendFactor,         // (1-R,  1-G,  1-B,  1-A ) by OMSetBlendFactor()
    InverseDestinationColor,    // (1-Rd, 1-Gd, 1-Bd, 1-Ad)
    InverseDestinationAlpha,    // (1-Ad, 1-Ad, 1-Ad, 1-Ad)
};


enum class BlendOperation
{
    Add,                        // Source1 + Source2
    Subtract,                   // Source1 - Source2
    ReverseSubtract,            // Source2 - Source1
    Min,                        // 最小値(Source1, Source2)
    Max,                        // 最大値(Source1, Source2)
};


enum class LogicOperation
{
    Clear,                      // 論理クリア      (0)
    Set,                        // 論理セット      (1)
    Copy,                       // 論理コピー      (s)
    CopyInverted,               // 論理コピー反転 !(s)
    Noop,                       // 論理No-Op       (d)
    Invert,                     // 論理反転       !(d)
    And,                        // 論理AND         (s & d) 
    NotAnd,                     // 論理NAND       !(s & d)
    Or,                         // 論理OR          (s | d)
    Nor,                        // 論理NOR        !(s | d)
    Xor,                        // 論理XOR         (s XOR d)
    Equivalence,                // 論理等価       !(s XOR d)
    AndReverse,                 // ANDの論理反転   (s & !d) 
    AndInverted,                // 論理反転AND    (!s &  d) 
    OrReverse,                  // 論理反転OR     ( s | !d)
    OrInverted,                 // 論理反転OR     (!s |  d)
};


class ColorWriteEnable
{
public:
    uint8_t mask;

public:
    static const uint8_t Red = 0b00000001;
    static const uint8_t Green = 0b00000010;
    static const uint8_t Blue = 0b00000100;
    static const uint8_t Alpha = 0b00001000;
    static const uint8_t All = 0b00001111;

public:
    ColorWriteEnable() : mask(0) {}
    ColorWriteEnable(uint8_t mask) : mask(mask) {}
    operator uint8_t() const { return mask; }
};


class RenderTargetBlend
{
public:
    // よく利用されるブレンド
    static const RenderTargetBlend Unused;
    static const RenderTargetBlend Opaque;
    static const RenderTargetBlend AlphaBlend;
    static const RenderTargetBlend Additive;
    static const RenderTargetBlend NonPremultiplied;

public:
    bool                    BlendEnable;
    bool                    LogicOpEnable;
    BlendFactor             SrcBlend;
    BlendFactor             DestBlend;
    BlendOperation          BlendOp;
    BlendFactor             SrcBlendAlpha;
    BlendFactor             DestBlendAlpha;
    BlendOperation          BlendOpAlpha;
    LogicOperation          LogicOp;
    ColorWriteEnable        RenderTargetWriteMask;

public:
    // コンストラクタ
    RenderTargetBlend();

    // コンストラクタ
    RenderTargetBlend(const RenderTargetBlend& source);

    // コンストラクタ
    RenderTargetBlend
    (
        bool                    blendEnable,
        bool                    logicOpEnable,
        BlendFactor             srcBlend,
        BlendFactor             destBlend,
        BlendOperation          blendOp,
        BlendFactor             srcBlendAlpha,
        BlendFactor             destBlendAlpha,
        BlendOperation          blendOpAlpha,
        LogicOperation          logicOp,
        ColorWriteEnable        renderTargetWriteMask
    );
};


