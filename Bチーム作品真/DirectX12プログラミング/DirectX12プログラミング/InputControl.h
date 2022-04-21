#pragma once
#include "PrimitiveValue.h"
#include "InputStateBlock.h"
#include <string>
#include <vector>
#include <list>
#include <cstdint>

// 前方宣言
class InputDevice;

// 抽象クラス
class InputControl
{
private:
    InputDevice* m_device;
    InputControl* m_parent;
    std::string m_name;
    std::string m_layout;
    std::string m_displayName;
    std::string m_shortDisplayName;
    std::vector<std::string> m_usages;
    std::list<InputControl*> m_children;

protected:
    InputStateBlock m_stateBlock;

protected:
    // コンストラクタ
    InputControl();

    // デバイスルートからこのステートまでの相対的なレベルを取得します。
    uint32_t GetStateOffsetRelativeToDeviceRoot() const;

    // 表示名を設定します。
    void SetDisplayName(const std::string& displayName) { m_displayName = displayName; }

    // 表示名を設定します。
    void SetShortDisplayName(const std::string& displayName) { m_shortDisplayName = displayName; }

    // 現在の状態へのポインタを取得します。
    void* GetCurrentStatePtr() const;

    // デフォルト状態へのポインタを取得します。
    void* GetDefaultStatePtr() const;

    // ノイズマスクへのポインタを取得します。
    void* GetNoiseMaskPtr() const;

    // 前フレームでの状態へのポインタを取得します。
    void* GetPreviousFrameStatePtr() const;

    // 
    virtual void RefreshConfiguration();

    void RefreshConfigurationIfNeeded();

    // コントロール階層が配置された後の最終的な初期化タスクを実行します。
    virtual void FinishSetup();

public:
    // エイリアス名の配列を取得します。
    std::vector<std::string> GetAliases() const;

    // 子コントローラーの配列を取得します。
    std::vector<InputControl*> GetChildren() const;

    // 所属する入力デバイスへの参照を取得します。
    InputDevice* GetDevice() const { return m_device; }

    // 表示名を取得します。
    const std::string& GetDisplayName() const { return m_displayName; }

    // 表示名を取得します。
    const std::string& GetShortDisplayName() const { return m_shortDisplayName; }

    // パスが一致するコントロールを取得します。
    InputControl* TryGetChildControl(const char* path) const;

    template<typename TControl>
    // パスが一致するコントロールを取得します。
    TControl* TryGetChildControl(const char* path) const;

    // パスが一致するコントロールを取得します。
    InputControl* operator[](const char* path) const { return TryGetChildControl(path); }

    // コントロール名を取得します。
    const std::string& GetName() const { return m_name; }

    // レイアウトを取得します。
    const std::string& GetLayout() const { return m_layout; }

    // コントロールにノイズが多い場合は true を返します。
    bool IsNoisy() const { return false; }

    // 親InputControlオブジェクトへの参照を取得します。
    InputControl* GetParent() const { return m_parent; }

    // ルートからのフルパスを取得します。
    std::string GetPath() const;

    // ステートブロックを取得します。
    InputStateBlock* GetStateBlock() { return &m_stateBlock; }

    // 実際の物理的コントロールを表していない場合は true を返します。
    bool IsSynthetic() const;

    // コントロールに関連付けられた利用タグの配列を取得します。
    std::vector<std::string> GetUsages() const;

    // 値のサイズを取得します。 (単位はバイト)
    virtual int32_t GetValueSizeInBytes() const = 0;

    //
    // virtual Type GetValueType() = 0;

    // レイアウトの変数をセミコロン区切りの文字列として取得します。
    std::string GetVariants() const;

    // 値を比較し、等価の場合は true を返します。
    virtual bool CompareValue(void *firstStatePtr, void *secondStatePtr) const = 0;

    // コントロールの作動範囲を表す正規化された値を計算します。 [0.0 ～ 1.0]
    float EvaluateMagnitude() const;

    // 与えられたステート内のコントロールの作動範囲を表す正規化された値を計算します。 [0.0 ～ 1.0]
    virtual float EvaluateMagnitude(void *statePtr) const;

    // 指定されたパスの子コントロールを取得します。
    InputControl* GetChildControl(const std::string& path) const;

    // 指定されたパスの子コントロールを取得します。
    template<typename TControl>
    TControl* GetChildControl(const std::string& path) const;

    //virtual object ReadValueFromBufferAsObject(void *buffer, int bufferSize) = 0;
    //virtual object ReadValueFromStateAsObject(void *statePtr) = 0;

    // 指定された状態からコントロールの最終的な処理済みの値を読み取り、指定されたバッファーに格納します。
    virtual void ReadValueFromStateIntoBuffer(void *statePtr, void *bufferPtr, int bufferSize) = 0;

    // 指定されたメモリから値を読み取り、状態として保存します。
    virtual void WriteValueFromBufferIntoState(void *bufferPtr, int bufferSize, void *statePtr);
};






template<typename TValue>
class InputControlT : public InputControl
{
public:
    // 値のサイズを取得します。 (単位はバイト)
    int32_t GetValueSizeInBytes() const override { return 0; }

    // 値を比較し、等価の場合は true を返します。
    bool CompareValue(void *firstStatePtr, void *secondStatePtr) const override { return false; }

    // 処理された値を読み取ります。
    TValue ProcessValue(const TValue& value) const;

    // デフォルト値を読み取ります。
    TValue ReadDefaultValue() const;

    // 処理されていない値を読み取ります。
    TValue ReadUnprocessedValue() const;

    // ステートから処理されていない値を読み取ります。
    virtual TValue ReadUnprocessedValueFromState(void *statePtr) = 0;

    // 値を読み取ります。
    TValue ReadValue() const;

    // 前フレームの値を読み取ります。
    TValue ReadValueFromPreviousFrame() const;

    // ステートから値を読み取ります。
    TValue ReadValueFromState(void *statePtr) const;

    // ステートから値を読み取り、指定されたバッファに格納します。
    void ReadValueFromStateIntoBuffer(void *statePtr, void *bufferPtr, int bufferSize) override { }

    // 指定されたバッファから値を読み取り、ステートに書き込みます。
    void WriteValueFromBufferIntoState(void *bufferPtr, int bufferSize, void *statePtr) override { }

    // 値をステートに書き込みます。
    virtual void WriteValueIntoState(const TValue& value, void *statePtr) { }

    //override object ReadValueFromBufferAsObject(void *buffer, int bufferSize);
    //override object ReadValueFromStateAsObject(void *statePtr);
    //override void WriteValueFromObjectIntoState(object value, void *statePtr);

};

