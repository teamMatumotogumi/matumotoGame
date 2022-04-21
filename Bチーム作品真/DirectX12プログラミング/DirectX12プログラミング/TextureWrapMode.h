#pragma once

enum class TextureWrapMode
{
    Repeat,     // テクスチャをタイルにする場合、繰り返すパターンとして作成します
    Clamp,      // 辺のピクセルが使用されます。
    Mirror,     // 整数境界毎にミラーリングパターンを繰り返します。
    MirrorOnce, // 一度だけミラーリングされます。それ以降は辺のピクセルが使用されます。
};

