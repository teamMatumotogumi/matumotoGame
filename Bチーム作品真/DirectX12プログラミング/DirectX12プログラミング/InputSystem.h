#pragma once
#include <string>
#include <vector>
#include "InputMetrics.h"
#include "InputSettings.h"

class InputDevice;
class InputDeviceDescription;

class InputRemoting
{
};

class Version
{
};

class InputSystem
{
private:
    static InputMetrics m_inputMetrics;
    static InputRemoting m_inputRemoting;
    static InputSettings m_inputSettings;
    static Version m_version;
    static float m_pollingFrequency;
    static std::vector<InputDevice*> m_devices;

public:
    // 現在接続されているデバイスのリストを取得します。
    static std::vector<InputDevice*> GetDevices();

    // 現在接続されていないデバイスのリストを取得します。
    static std::vector<InputDevice*> GetDisconnectedDevices();

    // 入力システムに関するさまざまな最新の測定値を取得します。
    static const InputMetrics& GetMetrics() { return m_inputMetrics; }

    // ポーリングが必要なデバイスがバックグラウンドで照会されている頻度を取得します。
    static float GetPollingFrequency() { return m_pollingFrequency; }

    // ポーリングが必要なデバイスがバックグラウンドで照会される頻度を設定します。
    static void SetPollingFrequency(float value) { m_pollingFrequency = value; }

    // ローカル入力をリモート入力システムにミラーリングできる、
    // またはリモートシステムの入力をローカルで使用可能にすることができるローカルInputRemotingインスタンスを取得します。
    static const InputRemoting& GetRemoting() { return m_inputRemoting; }

    // 入力システムの現在の構成を取得します。
    static const InputSettings& GetSettings() { return m_inputSettings; }

    // 入力システムの現在の構成を設定します。
    static void SetSettings(const InputSettings& settings) { m_inputSettings = settings; }

    // 入力システムパッケージの現在のバージョン取得します。
    static const Version& GetVersion() { return m_version; }

    // 指定されたレイアウト名で作成された新規デバイスをシステムに追加します。
    static InputDevice* AddDevice(const std::string& layout);

    // 指定されたレイアウト名で作成された新規デバイスをシステムに追加します。
    static InputDevice* AddDevice(const std::string& layout, const std::string& name);

    // 指定されたレイアウト名で作成された新規デバイスをシステムに追加します。
    static InputDevice* AddDevice(const std::string& layout, const std::string& name, const std::string& variants);

    // 指定されたデバイスをシステムに追加します。
    static void AddDevice(InputDevice* device);

    // 指定されたデバイス詳細を基にデバイスをシステムに追加します。
    static InputDevice* AddDevice(const InputDeviceDescription& description);

    // 指定された名前またはレイアウト名でデバイスを検索します。
    static InputDevice* GetDevice(const std::string& nameOrLayout);

    // 指定された型のデバイスを検索します。
    template<typename TDevice>
    static TDevice* GetDevice();

    // 指定された型のデバイスを検索します。
    template<typename TDevice>
    static TDevice* GetDevice(const std::string& usage);

    // 一意のIDでデバイスを検索します。
    static InputDevice GetDeviceById(int32_t deviceId);
};

