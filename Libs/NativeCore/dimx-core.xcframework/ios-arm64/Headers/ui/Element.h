#pragma once

#include "UICommon.h"
#include <Common.h>
#include <config/Config.h>
#include <FrameContext.h>
#include <Input.h>
#include <Color.h>
#include "Layout.h"
#include "UIScheme.h"
#include <LifeWatcher.h>
#include <Rect.h>
#include <ui/imgui/Font.h>

#include <EventPublisher.h>

DECL_ENUM(ElementEvent,  Click,   Show,   Hide,   Change,   Select,   WindowClose,   WindowLostFocus);
DECL_ESTR(ElementEvent, "Click", "Show", "Hide", "Change", "Select", "WindowClose", "WindowLostFocus");

class Dimension;
class ResourceManager;

namespace ui {

class UIContext;

DECLARE_PTR(Element);
class Element: public LifeWatcher
{
    DECLARE_EVENTS(ElementEvent)

    class Style {
    public:
        Style(const UIScheme* scheme, UIType type, const Config& config)
        : mScheme(scheme)
        , mType(type)
        , mConfig(config)
        {}

        template<typename T>
        T get(const std::string &key) const {
            if (const Config* node = mConfig.get<const Config*>(key, nullptr)) {
                return node->get<T>();
            }
            return mScheme->get<T>(mType, key);
        }

    private:
        const UIScheme* mScheme{nullptr};
        UIType mType{UIType::None};
        Config mConfig;
    };

public:
    Element(UIType type, UIContext* context, Element* parent, const Config& config);
    virtual ~Element();

    UIType uiType() const { return mUIType; }

    UIContext* context() { return mContext; }

    Element* parent() const { return mParent; }
    void setParent(Element* parent) { mParent = parent; }

    const Style& style() const { return mStyle; }

    const Config& customConfig() const { return mCustomConfig; }
    const std::string& name() const { return mName; }
    const std::string& text() const { return mText; }
    virtual void setText(const std::string& text) { mText = text; }

    const Font* font() const { return mFont; }
    float fontSize() const { return mFontSize; }
    void setFontSize(float size) { mFontSize = size; }

    const Vec4& color() const { return mColor; }
    void setColor(const Vec4& col) { mColor = col; }

    const Vec4& backgroundColor() const { return mBackgroundColor; }
    void setBackgroundColor(const Vec4& col) { mBackgroundColor = col; }

    void setTexture(const ObjectPtr& tex);
    const ObjectPtr& texture() const { return mTexture; }
    void setTexRect(const Rect& rect) { mTexRect = rect; }
    const Rect& texRect() const { return mTexRect; }

    Layout& layout() { return mLayout; }
    void setLayout(const Layout& layout) { mLayout = layout; }
    const Rect& regionPx() const { return mRegionPx; }

    bool visible() const { return mVisible; }
    void setVisible(const bool value);

    bool enabled() const { return mEnabled; }
    void setEnabled(bool value) { mEnabled = value; }

    const std::vector<ElementPtr>& elements() const { return mElements; }

    float alpha() const { return mAlpha; }
    void setAlpha(float alpha) { mAlpha = alpha; }

    float frameRounding() const { return mFrameRounding; }
    float frameBorderSize() const { return mFrameBorderSize; }
    
    void setBorderColor(const Vec4& col) { mBorderColor = col; }
    const Vec4& borderColor() const { return mBorderColor; }

    bool isShowBackground() const { return mShowBackground; }

    virtual void update(const FrameContext& frameContext);
    virtual bool processInputEvent(const InputEvent& event);

    ElementPtr addElement(const Config& config);
    void removeElement(const std::string& name);
    void removeAllElements();
    
    template <typename T = ui::Element>
    std::shared_ptr<T> find(const std::string& name, bool required = false) const;

    virtual void onClick();
    virtual void onButtonPressed(InputButton button, const Vec2& pos);
    virtual void onButtonReleased(InputButton button, const Vec2& pos);
    virtual void onCursorMove(CursorMove move, const Vec2& pos);
    virtual void onActivate(bool active);
    virtual void onChange();

    void updateLayout();

protected:
    Rect calcTexRect(const Rect& texRect, const Vec2& texSize);

private:
    UIType mUIType = UIType::None;
    UIContext* mContext = nullptr;
    Element* mParent = nullptr;
    Style mStyle;
    Config mCustomConfig;
    std::string mName;
    std::string mText;

    std::string mFontName;
    float mFontSize{-1};
    const Font* mFont{nullptr};

    Layout mLayout;

    Rect mRegionPx;

    bool mVisible = true;
    Vec4 mColor{Color::White};
    Vec4 mBackgroundColor{Color::None};
    ObjectPtr mTexture;
    Rect mTexRect{ 0, 1, 1, 0 };
    float mAlpha = 1.f;
    bool mEnabled = true;
    bool mShowBackground{true};

    float mFrameRounding{0.f};
    float mFrameBorderSize{0.f};
    Vec4 mBorderColor{1, 1, 1, 1};

    std::vector<ElementPtr> mElements;
};

////////////////////////////////////////////////////////////////////////////////

template <typename T>
std::shared_ptr<T> Element::find(const std::string& name, bool required) const {
    for (auto& elem : mElements) {
        ElementPtr result = elem->name() == name ? elem : elem->find<T>(name);
        if (result) {
            auto typedEl = std::dynamic_pointer_cast<T>(result);
            ASSERT(typedEl, "Failed to cast element [" << name << "]");
            return typedEl;
        }
    }
    ASSERT(required == false, "Failed to find element [" << name << "]");
    return {};
}

} // namespace ui
