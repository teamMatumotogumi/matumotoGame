#pragma once

enum class FilterMode
{
    Point,      // ピクセルを1つ1つブロックのように表示する。
    Bilinear,   // 平均化され滑らかに表示する。
    Trilinear,  // Bilinear とほぼ同じだがミップマップレベルにおいてブレンドして表示する
};
