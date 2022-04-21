#pragma once

// 深度ステンシルフォーマットを表す列挙型
enum class DepthStencilFormat
{
    Depth16,
    Depth24Stencil8,
    Depth32,
    Default = Depth32,
};
