#pragma once
#include "ButtonControl.h"
#include "Key.h"

class KeyControl : public ButtonControl
{
private:
    // この値はコントロールを所有するデバイスのFinishSetup（）によって初期化する必要があります。
    Key m_keyCode;

protected:
    // 
    void RefreshConfiguration() override;

public:
    // このキーのキーコードを取得します。
    Key GetKeyCode() const;

    // このキーのキーコードを設定します。
    void SetKeyCode(Key keyCode);

    // 基盤となるプラットフォームがキーを識別するために使用するコードを取得します。
    int32_t GetScanCode() const;
};

