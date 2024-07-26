#ifndef IOS_INPUT_H_INCLUDED
#define IOS_INPUT_H_INCLUDED
//---------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

void processTouchDown(float posX, float posY);
void processTouchMove(float posX, float posY);
void processTouchUp(float posX, float posY);

void processMultitouchDown(float posX, float posY);
void processMultitouchUp(float posX, float posY);

void processInsertText(long charCode);
void processDeleteBackward();

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
//---------------------------------------------------------------

#ifdef __cplusplus
#include <Input.h>

class IOSInput: public Input
{
public:
    void setInputMode(InputMode mode) override;
    void processInsertText(int charCode);
    void processOnCursorMove(const Vec2& pos);
    const Vec2& lastCursorPos() const { return mLastCursorPos; }

private:
    Vec2 mLastCursorPos;
};
#endif // __cplusplus

#endif // IOS_INPUT_H_INCLUDED
