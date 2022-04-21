#pragma once
#include "AxisControl.h"


class ButtonControl : public AxisControl
{
private:
    // 指定された値以上の場合にこのボタンが押されたと見なされます。
    // デフォルトでは -1 に設定されています。
    // この値が負の場合は defaultButtonPressPoint が使用されます。
    float m_pressPoint;
    float m_pressPointDefault;

public:
    // コンストラクタ
    ButtonControl();

    // このボタンが押されたと見なされる為の閾値を取得します。
    float GetPressPoint() const { return m_pressPoint; }

    // このボタンが押されたと見なされる為の閾値を設定します。
    void SetPressPoint(float value) { m_pressPoint = value; }

    // PressPointが未設定の場合のデフォルト値を取得します。
    float GetPressPointDefault() const { return m_pressPointDefault; }

    // このボタンがこのフレームで押されてた場合は true を返します。
    bool WasPressedThisFrame() const;

    // このボタンがこのフレームで離された場合は true を返します。
    bool WasReleasedThisFrame() const;

    // このボタンが現在押されている場合は true を返します。
    bool IsPressed() const;

    // 指定された値がこのボタンで押されたと見なされる場合は true を返します。
    bool IsValueConsideredPressed(float value);
};

