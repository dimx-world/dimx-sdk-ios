#ifndef IOS_DISPLAY_H_INCLUDED
#define IOS_DISPLAY_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

void Display_setSize(long width, long height);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

//===============================================================//

#ifdef __cplusplus
#include <Display.h>

class IOSDisplay : public Display
{
public:
    IOSDisplay();
    virtual ~IOSDisplay();
    void initialize(const Config& config) override;
    void deinitialize() override;
    void update(const FrameContext& frameContext) override;
    void endFrame(const FrameContext& frameContext) override;
};
#endif // __cplusplus

#endif // IOS_DISPLAY_H_INCLUDED