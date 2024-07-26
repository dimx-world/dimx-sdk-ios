#pragma once
#include <Common.h>

class Material;
class NativeMaterial
{
    NO_COPY_MOVE(NativeMaterial)
public:
    NativeMaterial(const Material& coreMaterial);
    virtual ~NativeMaterial() = default;

    const Material& coreMaterial() const;

    virtual void updateParamValue(const std::string& name, const MaterialValue& val);

    size_t id() const { return mId; }

protected:
    void setId(size_t id) { mId = id; }

private:
    const Material& mCoreMaterial;

    size_t mId{INVALID_INDEX};
};
