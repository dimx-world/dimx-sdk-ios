#pragma once
#include "Common.h"
#include "Input.h"
#include <world/Dimension.h>

//namespace PickOptions {
//const unsigned int DefaultDimension = 0;
//}

DECLARE_PTR(WorldPicker)
class WorldPicker: public InputHandler
{
public:
    WorldPicker();
    virtual ~WorldPicker();

    bool processEvent(const InputEvent& event) override;

    void update(const FrameContext& frameContext);
    
    const RaycastResult& raycastResult() const;

protected:
    virtual bool tryPick(const Vec2& screenPos);

private:
    Vec2 mScreenPos;
    Rect mScreenRect;

    RaycastResult mRaycastResult;
    // raycast matrix
};
