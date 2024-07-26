#pragma once
#include "Property.h"
#include <Common.h>
#include <functional>

DECL_ENUM(ValuePropertyType,  String,   Int,   UInt,   Float,   Double,   Vec2,   Vec3,   Vec4,   Bool,   List )
DECL_ESTR(ValuePropertyType, "String", "Int", "UInt", "Float", "Double", "Vec2", "Vec3", "Vec4", "Bool", "List")

namespace edit {

class ListReference
{
public:
    using Items = std::vector<std::string>;

    ListReference(const Items* items, int selected = -1)
        : mItems(items)
        , mSelected(selected)
    {
        ASSERT(items, "Invalid items");
    }

    const Items& items() const { return *mItems; }
    int selected() const { return mSelected; }

    void select(int selected) { mSelected = selected; }

private:
    const Items* mItems;
    int mSelected{ -1 };
};

template <typename T> ValuePropertyType getValuePropertyType();
template <> inline ValuePropertyType getValuePropertyType<std::string>()   { return ValuePropertyType::String; }
template <> inline ValuePropertyType getValuePropertyType<int>()           { return ValuePropertyType::Int;    }
template <> inline ValuePropertyType getValuePropertyType<unsigned int>()  { return ValuePropertyType::UInt;   }
template <> inline ValuePropertyType getValuePropertyType<float>()         { return ValuePropertyType::Float;  }
template <> inline ValuePropertyType getValuePropertyType<double>()        { return ValuePropertyType::Double; }
template <> inline ValuePropertyType getValuePropertyType<Vec2>()          { return ValuePropertyType::Vec2;   }
template <> inline ValuePropertyType getValuePropertyType<Vec3>()          { return ValuePropertyType::Vec3;   }
template <> inline ValuePropertyType getValuePropertyType<Vec4>()          { return ValuePropertyType::Vec4;   }
template <> inline ValuePropertyType getValuePropertyType<bool>()          { return ValuePropertyType::Bool;   }
template <> inline ValuePropertyType getValuePropertyType<ListReference>() { return ValuePropertyType::List;   }

DECLARE_PTR(ValueProperty)
class ValueProperty: public Property
{
    NO_COPY_MOVE(ValueProperty)
public:
    class BaseCallback {
    public:
        virtual ~BaseCallback() = default;
    };

    template <typename T>
    class Getter: public BaseCallback
    {
        using Callback = std::function<T()>;
    public:
        Getter(Callback&& cb)
        : mCallback(std::forward<Callback>(cb))
        {
            ASSERT(mCallback, "Invalid callback");
        }

        T get() const { return mCallback(); }
    private:
        std::function<T()> mCallback;
    };

    template <typename T>
    class Setter: public BaseCallback
    {
        using Callback = std::function<void(const T&)>;
    public:
        Setter(Callback&& cb)
        : mCallback(std::forward<Callback>(cb))
        {
        }

        void set(const T& val) const
        {
            ASSERT(mCallback, "Invalid callback");
            mCallback(val);
        }
    private:
        Callback mCallback;
    };

public:
    template <typename T>
    ValueProperty(const std::string& name,
                  std::function<T()> getter,
                  std::function<void(const T&)> setter)
    : Property(name)
    , mType(getValuePropertyType<T>())
    {
        ASSERT(mType != ValuePropertyType::None, "Invalid property type");

        mGetter = std::make_unique<Getter<T>>(std::move(getter));
        if (setter) {
            mSetter = std::make_unique<Setter<T>>(std::move(setter));
        }
        setReadOnly(mSetter ? false : true);
    }

    void update(ui::PropertyView& view) override;

    template <typename T>
    static std::shared_ptr<ValueProperty> create(
        std::string name,
        std::function<T()> getter,
        std::function<void(const T&)> setter)
    {
        return std::make_shared<ValueProperty>(std::move(name), std::move(getter), std::move(setter));
    }
private:
    template <typename T>
    T get() const
    {
        Getter<T>* typedGetter = dynamic_cast<Getter<T>*>(mGetter.get());
        ASSERT(typedGetter, "Invalid property type requested!");
        return typedGetter->get();
    }

    template <typename T>
    void set(const T& val)
    {
        ASSERT(mSetter, "ValueProperty is read only!");
        Setter<T>* typedSetter = dynamic_cast<Setter<T>*>(mSetter.get());
        ASSERT(typedSetter, "Invalid property type requested!");
        typedSetter->set(val);
        setModified(true);
    }

private:
    ValuePropertyType mType = ValuePropertyType::None;

    std::unique_ptr<BaseCallback> mGetter;
    std::unique_ptr<BaseCallback> mSetter;
};

} // namespace edit
