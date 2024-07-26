#include "IOSInput.h"
#include <Engine.h>
#include "IOSEngine.h"

void processTouchDown(float posX, float posY)
{
    if (!Engine::valid()) { return; }

    g_engine().pushEvent([pos = Vec2(posX, posY)] {
//        Vec2 pos = Vec2(posX, posY);
        static_cast<IOSInput&>(g_input()).processOnCursorMove(pos);
        g_input().onButtonPressed(InputButton::Button1, pos);
    });
}

void processTouchMove(float posX, float posY)
{
    if (!Engine::valid()) { return; }
    g_engine().pushEvent([pos = Vec2(posX, posY)] {
//    Vec2 pos = Vec2(posX, posY);
        float prevY = static_cast<IOSInput&>(g_input()).lastCursorPos().y;
        static_cast<IOSInput&>(g_input()).processOnCursorMove(pos);
        g_input().onWheel(Vec2{0, (pos.y - prevY) * 0.02f});
        prevY = pos.y;
    });
}

void processTouchUp(float posX, float posY)
{
    if (!Engine::valid()) { return; }
    g_engine().pushEvent([pos = Vec2(posX, posY)] {
//        Vec2 pos = Vec2(posX, posY);
        g_input().onButtonReleased(InputButton::Button1, pos);
    });
}

void processMultitouchDown(float posX, float posY)
{
    if (!Engine::valid()) { return; }
    g_engine().pushEvent([pos = Vec2(posX, posY)] {
//        Vec2 pos = Vec2(posX, posY);
        g_input().onButtonPressed(InputButton::MultiTouch, pos);
    });
}

void processMultitouchUp(float posX, float posY)
{
    if (!Engine::valid()) { return; }
    g_engine().pushEvent([pos = Vec2(posX, posY)] {
//        Vec2 pos = Vec2(posX, posY);
        g_input().onButtonReleased(InputButton::MultiTouch, pos);
    });
}

void processInsertText(long charCode)
{
    if (!Engine::valid()) { return; }
    g_engine().pushEvent([charCode] {
        static_cast<IOSInput&>(g_input()).processInsertText(static_cast<int>(charCode));
    });
}

void processDeleteBackward()
{
    if (!Engine::valid()) { return; }
    g_engine().pushEvent([] {
        g_input().onKeyPressed(InputKey::Backspace);
        g_input().onKeyReleased(InputKey::Backspace);
    });
}

//---------------------------------------------------------------------

void IOSInput::setInputMode(InputMode mode)
{
    if (mode == InputMode::Text) {
        g_swiftEngine()->showKeyboard();
    } else if (inputMode() == InputMode::Text) {
        g_swiftEngine()->hideKeyboard();
    }

    Input::setInputMode(mode);
}

void IOSInput::processInsertText(int charCode)
{
    switch (charCode) {
        case 10: {
            onKeyPressed(InputKey::Enter);
            onKeyReleased(InputKey::Enter);
        }
        default: {
            onCharInput(charCode);
        }
    }
}

void IOSInput::processOnCursorMove(const Vec2& pos)
{
    Input::onCursorMoved(pos);
    mLastCursorPos = pos;
}
