#pragma once
#include <string>

struct InputDeviceDescription
{
private:
    bool m_isEmpty = true;
    std::string m_capabilities;
    std::string m_deviceClass;
    std::string m_interfaceName;
    std::string m_manufacturer;
    std::string m_product;
    std::string m_serial;
    std::string m_version;

public:
    // デバイス固有の機能を一覧表示するオプションのJSON文字列を取得します。
    const std::string& GetCapabilities() const { return m_capabilities; }

    // デバイス固有の機能を一覧表示するオプションのJSON文字列を設定します。
    void SetCapabilities(const std::string& capabilities) { m_capabilities = capabilities; m_isEmpty = true; }

    // デバイスクラス文字列を取得します。
    const std::string& GetDeviceClass() const { return m_deviceClass; }

    // デバイスクラス文字列を設定します。
    void SetDeviceClass(const std::string& deviceClass) { m_deviceClass = deviceClass; m_isEmpty = true; }

    // インターフェイス名を取得します。
    const std::string& GetInterfaceName() const { return m_interfaceName; }

    // インターフェイス名を設定します。
    void SetInterfaceName(const std::string& interfaceName) { m_interfaceName = interfaceName; m_isEmpty = true; }

    // デバイスを製造したベンダー名を取得します。
    const std::string& GetManufacturer() const { return m_manufacturer; }

    // デバイスを製造したベンダー名を設定します。
    void SetManufacturer(const std::string& manufacturer) { m_manufacturer = manufacturer; m_isEmpty = true; }

    // ベンダーによってデバイスに割り当てられた製品名を取得します。
    const std::string& GetProduct() const { return m_product; }

    // ベンダーによってデバイスに割り当てられた製品名を設定します。
    void SetProduct(const std::string& product) { m_product = product; m_isEmpty = true; }

    // デバイスのシリアル番号を取得します。
    const std::string& GetSerial() const { return m_serial; }

    // デバイスのシリアル番号を設定します。
    void SetSerial(const std::string& serial) { m_serial = serial; m_isEmpty = true; }

    // デバイスやドライバーのバージョンを取得します。
    const std::string& GetVersion() const { return m_version; }

    // デバイスやドライバーのバージョンを設定します。
    void SetVersion(const std::string& version) { m_version = version; m_isEmpty = true; }

    // インターフェイス名, デバイスクラス, ベンダー名, 製品名, シリアル番号, バージョン, デバイス固有の機能
    // のいずれかの情報が設定されている場合は true を返します。
    bool IsEmpty() const { return m_isEmpty; }
};
