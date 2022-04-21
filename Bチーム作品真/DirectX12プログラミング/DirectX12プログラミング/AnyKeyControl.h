#pragma once
#include "ButtonControl.h"

class AnyKeyControl : public ButtonControl
{
public:
    // コンストラクタ
    // デフォルトの初期化。
    // 状態サイズを1ビットに設定し、フォーマットをFormatBitに設定します。
    AnyKeyControl();

    // オーバーライド
    float ReadUnprocessedValueFromState(void *statePtr) override;
};
