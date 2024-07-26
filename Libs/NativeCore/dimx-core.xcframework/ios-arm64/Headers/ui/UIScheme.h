#pragma once

#include "UICommon.h"
#include <Common.h>
#include <config/Config.h>

namespace ui {

class UIScheme {
public:
    UIScheme(UIScheme* parent, const Config &config)
    : mParent(parent)
    , mConfig(config)
    {
        mElementConfig = mConfig.get<const Config*>("Element", nullptr);
    }

    template<typename T = std::string>
    T get(UIType type, const std::string &key) const
    {
        const Config* typeConfig = mConfig.get<const Config*>(enum2str(type), nullptr);
        const Config* node = typeConfig ? typeConfig->get<const Config*>(key, nullptr) : nullptr;
        if (node) {
            return node->get<T>();
        }

        node = mElementConfig ? mElementConfig->get<const Config*>(key, nullptr) : nullptr;

        if (!node && mParent) {
            return mParent->get<T>(type, key);
        }

        ASSERT(node, "Failed to lookup ui scheme value " << type << ":" << key);
        return node->get<T>();
    }

    float pointSize() const {
        return 0.01f; // 1cm
    }

private:
    UIScheme* mParent{nullptr};
    Config mConfig;
    const Config* mElementConfig{nullptr};
};

} // namespace ui
