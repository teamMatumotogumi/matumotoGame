#pragma once
#include <cstdint>
#include <windows.h>

//---------------------------------------------------------------------------------------------------------------------------------------------
// キーボード
//
//   ・WindowsAPIを使ったデバイス入力システム。
//   ・キーの状態を取得することができる。
// 
//---------------------------------------------------------------------------------------------------------------------------------------------
class Keyboard
{
private:
    static uint8_t m_previous[256];
    static uint8_t m_current[256];

public:
    static void Initialize();
    static void Update();
    static bool Pressed(int32_t keyCode) { return (m_current[keyCode] & 0x80) == 0x80; }
    static bool Released(int32_t keyCode) { return (m_current[keyCode] & 0x80) != 0x80; }
    static bool JustPressed(int32_t keyCode) { return !(m_previous[keyCode] & 0x80) && (m_current[keyCode] & 0x80); }
    static bool JustReleased(int32_t keyCode) { return (m_previous[keyCode] & 0x80) && !(m_current[keyCode] & 0x80); }
};

