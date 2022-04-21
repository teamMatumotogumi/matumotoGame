#pragma once
#include "InputControl.h"
#include "InputDeviceDescription.h"

class InputDevice : public InputControl
{
public:
    static constexpr int32_t InvalidDeviceId = 0;

    // これは、デバイスがシステムに追加された後にのみ割り当てられます。
    // デバイスが取り外された場合でも、2つのデバイスが同じIDを受け取ることはなく、別のデバイスが以前に使用したIDを受け取るデバイスもありません。
    // これに対する唯一の例外は、レイアウト変更の一部としてデバイスが再作成される場合です。
    // たとえば、マウスレイアウトを置き換える新しいレイアウトが登録されている場合、すべてのマウスデバイスが再作成されますが、既存のデバイスIDは保持されます。
    int32_t m_deviceId;
    bool m_isAdded;
    bool m_isEnabled;
    bool m_isNative;
    bool m_isRemote;
    double m_lastUpdateTime;
    InputDeviceDescription m_description;

protected:
    // 
    virtual void OnRemoved() { }

public:
    // コンストラクタ
    InputDevice();

    // この入力デバイスがInputSystemに所属していている場合は true を返します。
    bool IsAdded() const { return m_isAdded; }

    // 全てのコントロールの配列を返します。
    std::vector<InputControl*> GetAllControls() const;

    // アプリケーションがバックグラウンド中でも入力を配信できる場合は true を返します。
    bool CanRunInBackground() const { return false; }

    // この入力デバイスの詳細情報を取得します。
    const InputDeviceDescription& GetDescription() const { return m_description; }

    // この入力デバイスのユニークIDを取得します。
    int32_t GetDeviceId() const { return m_deviceId; }

    // この入力デバイスが現在有効な場合は true を返します。
    // 無効になっているデバイスはイベントを受信しません。 
    // デバイスに送信されているイベントは無視されます。
    bool IsEnabled() const { return m_isEnabled; }

    // 最終更新時間を取得します。
    double GetLastUpdateTime() const { return m_lastUpdateTime; }

    // ランタイムによって報告されたデバイスは true を返しますが、
    // 手動(AddDeviceメソッド)で追加されたデバイスは false を返します。
    bool IsNative() const { return m_isNative; }

    // デバイスがリモート入力システムからミラーリングされており、
    // ローカルシステムに「実際の」デバイスとして実際に存在していない場合は true を返します。
    bool IsRemote() const { return m_isRemote; }

    // 
    virtual void MakeCurrent() { }
};
