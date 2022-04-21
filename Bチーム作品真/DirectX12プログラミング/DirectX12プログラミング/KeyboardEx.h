#pragma once
#include "InputDevice.h"

class ITextInputReceiver
{


};


// 前方宣言
class ButtonControl;
class KeyControl;
class AnyKeyControl;
enum class Key;

class KeyboardEx
    : public InputDevice
    , public ITextInputReceiver
{
protected:
    void SetKeyboardLayout(const std::string& layout);
    void FinishSetup() override;
    void OnRemoved() override;
    void RefreshConfiguration() override;

public:
    static constexpr uint32_t KeyCount = 110;
    static KeyboardEx* current;

    // レイアウト文字列を取得します。
    const std::string& _GetKeyboardLayout() const;

    // 現在アクティブなキーボードを取得します。
    static KeyboardEx* _GetCurrent() { return current; }

    // 全てのキーのコントロールを取得します。
    std::vector<KeyControl*> _AllKeys() const;

    // 任意のキーのコントロールを取得します。
    KeyControl& _Item(Key key) const;

    // 
    KeyControl& FindKeyOnCurrentKeyboardLayout(const std::string& displayName);

    void MakeCurrent() override;

    void OnTextInput(char character);

    //void OnIMECompositionChanged(IMECompositionString compositionString);

    void SetIMECursorPosition(const Vector2& position);

    void SetIMEEnabled(bool enabled);

    //event Action<IMECompositionString> onIMECompositionChange;
    //event Action<char> onTextInput;

    KeyControl& A() const;
    ButtonControl& Alt() const;
    AnyKeyControl& Any() const;
    KeyControl& Backquote() const;
    KeyControl& Backslash() const;
    KeyControl& Backspace() const;
    KeyControl& B() const;
    KeyControl& CapsLock() const;
    KeyControl& C() const;
    KeyControl& Comma() const;
    KeyControl& ContextMenu() const;
    ButtonControl& Ctrl() const;
    KeyControl& Delete() const;
    KeyControl& Digit0() const;
    KeyControl& Digit1() const;
    KeyControl& Digit2() const;
    KeyControl& Digit3() const;
    KeyControl& Digit4() const;
    KeyControl& Digit5() const;
    KeyControl& Digit6() const;
    KeyControl& Digit7() const;
    KeyControl& Digit8() const;
    KeyControl& Digit9() const;
    KeyControl& D() const;
    KeyControl& DownArrow() const;
    KeyControl& E() const;
    KeyControl& End() const;
    KeyControl& Enter() const;
    KeyControl& Equals() const;
    KeyControl& Escape() const;
    KeyControl& F10() const;
    KeyControl& F11() const;
    KeyControl& F12() const;
    KeyControl& F1() const;
    KeyControl& F2() const;
    KeyControl& F3() const;
    KeyControl& F4() const;
    KeyControl& F5() const;
    KeyControl& F6() const;
    KeyControl& F7() const;
    KeyControl& F8() const;
    KeyControl& F9() const;
    KeyControl& F() const;
    KeyControl& G() const;
    KeyControl& H() const;
    KeyControl& Home() const;
    KeyControl& I() const;
    ButtonControl& ImeSelected() const;
    KeyControl& Insert() const;
    KeyControl& J() const;
    KeyControl& K() const;
    KeyControl& LeftAlt() const;
    KeyControl& LeftApple() const;
    KeyControl& LeftArrow() const;
    KeyControl& LeftBracket() const;
    KeyControl& LeftCommand() const;
    KeyControl& LeftCtrl() const;
    KeyControl& LeftMeta() const;
    KeyControl& LeftShift() const;
    KeyControl& LeftWindows() const;
    KeyControl& L() const;
    KeyControl& Minus() const;
    KeyControl& M() const;
    KeyControl& N() const;
    KeyControl& NumLock() const;
    KeyControl& Numpad0() const;
    KeyControl& Numpad1() const;
    KeyControl& Numpad2() const;
    KeyControl& Numpad3() const;
    KeyControl& Numpad4() const;
    KeyControl& Numpad5() const;
    KeyControl& Numpad6() const;
    KeyControl& Numpad7() const;
    KeyControl& Numpad8() const;
    KeyControl& Numpad9() const;
    KeyControl& NumpadDivide() const;
    KeyControl& NumpadEnter() const;
    KeyControl& NumpadEquals() const;
    KeyControl& NumpadMinus() const;
    KeyControl& NumpadMultiply() const;
    KeyControl& NumpadPeriod() const;
    KeyControl& NumpadPlus() const;
    KeyControl& Oem1() const;
    KeyControl& Oem2() const;
    KeyControl& Oem3() const;
    KeyControl& Oem4() const;
    KeyControl& Oem5() const;
    KeyControl& O() const;
    KeyControl& PageDown() const;
    KeyControl& PageUp() const;
    KeyControl& Pause() const;
    KeyControl& Period() const;
    KeyControl& P() const;
    KeyControl& PrintScreen() const;
    KeyControl& Q() const;
    KeyControl& Quote() const;
    KeyControl& RightAlt() const;
    KeyControl& RightApple() const;
    KeyControl& RightArrow() const;
    KeyControl& RightBracket() const;
    KeyControl& RightCommand() const;
    KeyControl& RightCtrl() const;
    KeyControl& RightMeta() const;
    KeyControl& RightShift() const;
    KeyControl& RightWindows() const;
    KeyControl& R() const;
    KeyControl& ScrollLock() const;
    KeyControl& Semicolon() const;
    ButtonControl& Shift() const;
    KeyControl& S() const;
    KeyControl& Slash() const;
    KeyControl& Space() const;
    KeyControl& Tab() const;
    KeyControl& T() const;
    KeyControl& U() const;
    KeyControl& UpArrow() const;
    KeyControl& V() const;
    KeyControl& W() const;
    KeyControl& X() const;
    KeyControl& Y() const;
    KeyControl& Z() const;
};
