#pragma once
#include <Common.h>
#include <config/Config.h>
#include <Rect.h>

namespace ui {
class PropertyView;
}

DECL_ENUM(PropertyFlag,  ReadOnly,   Modified);
DECL_ESTR(PropertyFlag, "ReadOnly", "Modified");

class PropertyFlags
{
public:
    bool isSet(PropertyFlag flag) const {
        return mFlags & flagMask(flag);
    }

    void set(PropertyFlag flag, bool value) {
        if (value) {
            mFlags |= flagMask(flag);
        } else {
            mFlags &= ~flagMask(flag);
        }
    }

private:
    uint64_t flagMask(PropertyFlag flag) const {
        return 1ULL << static_cast<int>(flag);
    }

private:
    uint64_t mFlags{0};
};

namespace edit {

DECLARE_PTR(Property)
class Property {
    NO_COPY_MOVE(Property)
public:
    Property(const std::string& name);
    virtual ~Property() = default;

    virtual void update(ui::PropertyView& view) {};

    const std::string& name() const;

    void setVisible(bool val);
    bool visible() const;

    void setReadOnly(bool val);
    bool readOnly() const;

    virtual bool modified() const;
    virtual void setModified(bool flag);

    PropertyFlags flags() const;

private:
    std::string mName;

    bool mModified = false;

    bool mVisible = true;
    bool mReadOnly = false;

    PropertyFlags mFlags;
};

} // namespace edit
