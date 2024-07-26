#pragma once
#include <Common.h>
#include <File.h>

#include <ozz/animation/runtime/skeleton.h>

DECLARE_PTR(Skeleton)
class Skeleton
{
public:
    Skeleton(const Buffer& data);
    const ozz::animation::Skeleton& ozzSkeleton() const;
private:
    ozz::animation::Skeleton mOzzSkeleton;
};
