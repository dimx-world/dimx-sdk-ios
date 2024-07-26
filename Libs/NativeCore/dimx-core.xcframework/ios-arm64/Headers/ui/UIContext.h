#pragma once
#include "Element.h"
#include "UIScheme.h"

class Dimension;
class Location;
namespace ui {

class UIContext
{
public:
    UIContext(Dimension* dim, Location* loc, const Config& config);

    Dimension* dimension() { return mDimension; }
    Location* location() { return mLocation; }

    ResourceManager& resourceManager();

    const UIScheme& scheme() const { return mScheme; }

    const ElementPtr& root() const { return mRoot; }

private:
    Dimension* mDimension = nullptr;
    Location* mLocation = nullptr;
    UIScheme mScheme;
    ElementPtr mRoot;
};

} // namespace ui