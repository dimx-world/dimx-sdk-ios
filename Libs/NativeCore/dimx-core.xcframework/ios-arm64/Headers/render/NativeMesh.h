#pragma once
#include <Common.h>

class Mesh;
class NativeMesh
{
    NO_COPY_MOVE(NativeMesh)
public:
    NativeMesh(const Mesh& coreMesh);
    virtual ~NativeMesh() = default;

    const Mesh& coreMesh() const;

    size_t id() const { return mId; }

protected:
    void setId(size_t id) { mId = id; }

private:
    const Mesh& mCoreMesh;
    size_t mId{INVALID_INDEX};
};
