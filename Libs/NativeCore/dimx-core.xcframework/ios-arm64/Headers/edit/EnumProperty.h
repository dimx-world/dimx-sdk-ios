#pragma once
#include "Property.h"
#include <ui/PropertyView.h>
#include <Common.h>
#include <functional>

namespace edit {

template <typename T>
class EnumProperty: public Property
{
    NO_COPY_MOVE(EnumProperty)
public:
    EnumProperty(const std::string& name,
                 bool showEmptyValue,
                 std::function<T()> getter,
                 std::function<void(T)> setter)
    : Property(name)
    , mShowEmptyValue(showEmptyValue)
    , mGetter(std::move(getter))
    , mSetter(std::move(setter))
    {
        ASSERT(mGetter, "Must have getter");
        setReadOnly(mSetter ? false : true);

        mList.reserve(static_cast<size_t>(T::Max) + 1);
        if (mShowEmptyValue) {
            mList.push_back({});
        }
        for (size_t i = 0; i < static_cast<size_t>(T::Max); ++i) {
            mList.push_back(enum2str(static_cast<T>(i)));
        }
    }

    void update(ui::PropertyView& view) override
    {
        T val = mGetter();
        int selected = static_cast<int>(val);
        if (mShowEmptyValue) {
            selected = val == T::None ? 0 : static_cast<int>(val) + 1;
        }

        if (selected >= static_cast<int>(mList.size())) {
            selected = -1;
        }

        if (view.addComboBox(name(), mList, selected, flags())) {
           if (mSetter)  {
                T newVal = static_cast<T>(selected);
                if (mShowEmptyValue) {
                    newVal = selected == 0 ? T::None : static_cast<T>(selected - 1);
                }
                mSetter(newVal);
                setModified(true);
           }
        }
    }

private:
    bool mShowEmptyValue{true};
    std::vector<std::string> mList;
    std::function<T()> mGetter;
    std::function<void(T)> mSetter;
};

} // namespace edit
