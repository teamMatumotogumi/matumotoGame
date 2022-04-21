#pragma once
#include "InputControl.h"

class AxisControl : public InputControlT<float>
{
public:
    enum class Clamp
    {
        None,                       // クランプしない
        BeforeNormalize,            // 正規化前にクランプする
        AfterNormalize,             // 正規化後にクランプする
        ToConstantBeforeNormalize,  // 正規化前に定数値にする
    };

private:
    Clamp m_clamp;
    float m_clampConstant;
    float m_clampMin;
    float m_clampMax;
    float m_normalizeMin;
    float m_normalizeMax;
    float m_normalizeZero;
    float m_scaleFactor;
    bool m_invert;
    bool m_normalize;
    bool m_scale;

public:
    // コンストラクタ
    AxisControl();

    // クランプ方法を設定します。
    void SetClamp(Clamp clamp)  { m_clamp = clamp; }

    // クランプ方法を取得します。
    Clamp GetClamp() const { return m_clamp; }

    // Clamp に ToConstantBeforeNormalizeが設定されている時に使用される定数値を設定します。
    void SetClampConstant(float value) { m_clampConstant = value; }

    // Clamp に ToConstantBeforeNormalizeが設定されている時に使用される定数値を取得します。
    float GetClampConstant() const { return m_clampConstant; }

    // クランプ範囲の最小値を設定します。
    void SetClampMin(float value) { m_clampMin = value; }

    // クランプ範囲の最小値を取得します。
    float GetClampMin() const { return m_clampMin; }

    // クランプ範囲の最大値を設定します。
    void SetClampMax(float value) { m_clampMax = value; }

    // クランプ範囲の最大値を取得します。
    float GetClampMax() const { return m_clampMax; }

    // 入力値が正規化される時の最小値を取得します。
    float GetNormalizeMin() const { return m_normalizeMin; }

    // 入力値が正規化される時の最大値を取得します。
    float GetNormalizeMax() const { return m_normalizeMax; }

    // 入力値が正規化される時の0基点の値を取得します。
    float GetNormalizeZero() const { return m_normalizeZero; }

    // 入力値が正規化される時の最小値を設定します。
    void SetNormalizeMin(float value) { m_normalizeMin = value; }

    // 入力値が正規化される時の最大値を設定します。
    void SetNormalizeMax(float value) { m_normalizeMax = value; }

    // 入力値が正規化される時の0基点の値を設定します。
    void SetNormalizeZero(float value) { m_normalizeZero = value; }

    // 入力値がスケーリングされる際の係数を設定します。
    void SetScaleFactor(float value) { m_scaleFactor = value; }

    // 入力値がスケーリングされる際の係数を設定します。
    float  GetScaleFactor() const { return m_scaleFactor; }

    // 入力値を反転したい場合は true を設定します。
    void IsInvert(bool enable) { m_invert = enable; }

    // 入力値が反転されている場合は true を返します。
    bool IsInvert() const { return m_invert; }

    // 入力値を正規化したい場合は true を設定します。
    void IsNormalize(bool enable) { m_normalize = enable; }

    // 入力値が正規化されている場合は true を返します。
    bool IsNormalize() const { return m_normalize; }

    // 入力値をスケーリングしたい場合は true を設定します。
    void IsScale(bool enable) { m_scale = enable; }

    // 入力値がスケーリングされている場合は true を返します。
    bool IsScale() const { return m_scale; }

    // 値を比較し、等価の場合は true を返します。
    bool CompareValue(void *firstStatePtr, void *secondStatePtr) const override;

    // 与えられたステート内のコントロールの作動範囲を表す正規化された値を計算します。 [0.0 ～ 1.0]
    float EvaluateMagnitude(void *statePtr) const override;

    // ステートから処理されていない値を読み取ります。
    float ReadUnprocessedValueFromState(void *statePtr) override;

    // 値をステートに書き込みます。
    void WriteValueIntoState(const float& value, void *statePtr) override;

protected:
    //
    void FinishSetup() override;

    // 
    float Preprocess(float value);

};

