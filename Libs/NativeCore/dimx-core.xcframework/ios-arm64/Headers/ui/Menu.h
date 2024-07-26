#pragma once
#include "Element.h"
#include <imgui_internal.h>

namespace ui {

class MenuHandler: public std::enable_shared_from_this<MenuHandler>
{
    using ClickCallback = std::function<void(size_t)>;
    using StrToIdCallback = std::function<size_t(const std::string&)>;
    struct Item {
        Item() = default;
        Item(const Config& config, const StrToIdCallback& str2id) {
            id = str2id(config.get("id"));
            text = config.get("text");
            visible = config.get("visible", visible);
            enabled = config.get("enabled", enabled);
        }

        size_t id{INVALID_INDEX};
        std::string text;
        bool visible{true};
        bool enabled{true};
    };

public:
    MenuHandler(const Config& config, StrToIdCallback str2id, ClickCallback callback)
    : mCallback(std::move(callback))
    {
        mItemHeight = config.get("item_height", 0.0f);
        if (mItemHeight == 0.f) {
            mItemHeight = ImGui::GetTextLineHeightWithSpacing();
        }
        mLayout.initialize(config.get<Config&>("layout"));
        Config empty;
        for (const Config& cfg: config.get<Config&>("items")) {
            addItem(Item(cfg, str2id));
        }
    }

    const Layout& layout() const { return mLayout; };
    Layout& layout() { return mLayout; };

    const std::vector<Item>& items() const { return mItems; }
    void setItemVisible(size_t id, bool visible) {
        ASSERT(id < mItems.size(), "Menu item id is out of bounds (visible)! id: " << id << ", num items: " << mItems.size());
        mItems[id].visible = visible;
    }

    void setItemEnabled(size_t id, bool enabled) {
        ASSERT(id < mItems.size(), "Menu item id is out of bounds (enabled)! id: " << id << ", num items: " << mItems.size());
        mItems[id].enabled = enabled;
    }

    void setItemText(size_t id, const std::string& text) {
        ASSERT(id < mItems.size(), "Menu item id is out of bounds (text)! id: " << id << ", num items: " << mItems.size());
        mItems[id].text = text;
    }

    float itemHeight() const { return mItemHeight; }
    void onClick(size_t id) {
        if (mCallback) {
            mCallback(id);
        }
    }

private:
    void addItem(Item&& item) {
        if (item.id >= mItems.size()) {
            mItems.resize(item.id + 1);
            mItems[item.id] = std::move(item);
        }
    }

private:
    Layout mLayout;
    std::vector<Item> mItems;
    float mItemHeight{0};
    ClickCallback mCallback;
};


DECLARE_PTR(Menu)
class Menu : public Element
{
public:
    struct Item {
        Item() = default;
        Item(const std::string& nm, const std::string& txt);
        Item(const Config& config);
        std::string name;
        std::string text;
        bool visible{true};
        bool enabled{true};
    };

public:
    Menu(UIContext* context, Element* parent, const Config& config);
    virtual ~Menu() = default;
    void update(const FrameContext& frameContext) override;

    int findItem(const std::string& name);
    void setItemText(int item, const std::string& text);
    void setItemVisible(int item, bool visible);

    void clearItems();
    Item& addItem(const std::string& name, const std::string& text);

    void setHandler(MenuHandler* handler) { mHandler = handler; }

private:
    bool updateItem(Item& item);

private:
    float mItemHeight{0};
    std::vector<Item> mItems;

    MenuHandler* mHandler{nullptr};
};

} // namespace