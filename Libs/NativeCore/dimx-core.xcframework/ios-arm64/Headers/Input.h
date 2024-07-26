#pragma once

#ifndef DIMX_CORE_INPUT_H
#define DIMX_CORE_INPUT_H

#include "CrossObject.h"
#include <config/Config.h>
#include "FrameContext.h"
#include <array>

DECL_ENUM(InputKey,  A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
                     Space, Apostrophe, Comma, Minus, Period, Slash,
                     Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
                     Semicolon, Equal, LeftBracket, BackSlash, RightBracket, Accent,
                     Escape, Enter, Tab, Backspace, Insert, Delete, Right, Left, Down, Up,
                     PageUp, PageDown, Home, End, CapsLock, ScrollLock, NumLock, PrintScreen, Pause,
                     F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
                     F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25,
                     KP0, KP1, KP2, KP3, KP4, KP5, KP6, KP7, KP8, KP9,
                     KPDecimal, KPDivide, KPMultiply, KPSubtract, KPAdd, KPEnter, KPEqual,
                     LeftShift, LeftControl, LeftAlt, LeftSuper, RightShift, RightControl, RightAlt, RightSuper, Menu)
DECL_ESTR(InputKey, "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
                    "Space", "Apostrophe", "Comma", "Minus", "Period", "Slash",
                    "Num0", "Num1", "Num2", "Num3", "Num4", "Num5", "Num6", "Num7", "Num8", "Num9",
                    "Semicolon", "Equal", "LeftBracket", "BackSlash", "RightBracket", "Accent",
                    "Escape", "Enter", "Tab", "Backspace", "Insert", "Delete", "Right", "Left", "Down", "Up",
                    "PageUp", "PageDown", "Home", "End", "CapsLock", "ScrollLock", "NumLock", "PrintScreen", "Pause",
                    "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
                    "F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20", "F21", "F22", "F23", "F24", "F25",
                    "KP0", "KP1", "KP2", "KP3", "KP4", "KP5", "KP6", "KP7", "KP8", "KP9",
                    "KPDecimal", "KPDivide", "KPMultiply", "KPSubtract", "KPAdd", "KPEnter", "KPEqual",
                    "LeftShift", "LeftControl", "LeftAlt", "LeftSuper", "RightShift", "RightControl", "RightAlt", "RightSuper", "Menu")

DECL_ENUM(KeyAction,  Pressed,   Released,   Repeated)
DECL_ESTR(KeyAction, "Pressed", "Released", "Repeated")

DECL_ENUM(KeyFlag,  Shift,   Control,   Alt)
DECL_ESTR(KeyFlag, "Shift", "Control", "Alt")

DECL_ENUM(InputEventType,  ButtonPressed,   ButtonReleased,   CursorMoved,   KeyPressed,   KeyReleased,   CharInput,   Wheel)
DECL_ESTR(InputEventType, "ButtonPressed", "ButtonReleased", "CursorMoved", "KeyPressed", "KeyReleased", "CharInput", "Wheel")

DECL_ENUM(InputButton,  Button1,   Button2,   Button3,   MultiTouch)
DECL_ESTR(InputButton, "Button1", "Button2", "Button3", "MultiTouch")

DECL_ENUM(CursorMove,  In,   Out,   Within)
DECL_ESTR(CursorMove, "In", "Out", "Within")

DECL_ENUM(InputMode,  Normal,   Text)
DECL_ESTR(InputMode, "Normal", "Text")

struct InputEvent
{
    InputEventType type{InputEventType::None};
    InputButton button{InputButton::None};
    Vec2 position{0.f, 0.f};
    InputKey key{InputKey::None};
    unsigned int keyFlags{0};
    unsigned int inputChar{0};
};

DECLARE_PTR(InputHandler)
class InputHandler
{
public:
    virtual ~InputHandler(){}
    virtual bool processEvent(const InputEvent& event) = 0;

    bool active() const { return mActive; }

protected:
    void activate(bool active) { mActive = active; }

private:
    bool mActive = false;
};

class Input: public CrossObject
{
    using ButtonEventsArray = std::array<bool, static_cast<size_t>(InputButton::Max) + static_cast<size_t>(InputKey::Max)>;
    
public:
    Input();
    virtual ~Input();

    virtual void initialize(const Config& config);
    virtual void deinitialize();
    virtual void update(const FrameContext& frameContext);

    virtual void setInputMode(InputMode mode);
    InputMode inputMode() const;

    static bool checkKeyFlag(unsigned int flags, KeyFlag keyFlag);
    static unsigned int setKeyFlag(unsigned int flags, KeyFlag keyFlag);
    static unsigned char keyToAscii(InputKey key);
    static InputKey keyFromAscii(unsigned char ascii);

    void onButtonPressed(InputButton button, const Vec2& scrnPos, unsigned int flags = 0);
    void onButtonReleased(InputButton button, const Vec2& scrnPos, unsigned int flags = 0);
    void onCursorMoved(const Vec2& scrnPos, unsigned int flags = 0);
    void onWheel(const Vec2& offset, unsigned int flags = 0);

    void onKeyPressed(InputKey key, unsigned int flags = 0);
    void onKeyReleased(InputKey key, unsigned int flags = 0);

    void onCharInput(int charCode);

private:
    bool processInputEvent(const InputEvent& event);

private:
    InputMode mInputMode = InputMode::Normal;
    ButtonEventsArray mFrameButtonEvents;
};

#endif // DIMX_CORE_INPUT_H