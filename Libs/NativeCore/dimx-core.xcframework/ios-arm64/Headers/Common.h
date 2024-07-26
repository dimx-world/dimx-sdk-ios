#pragma once

#ifndef DIMX_CORE_COMMON_H
#define DIMX_CORE_COMMON_H

#include "Version.h"

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <limits>
#include <algorithm>
#include <variant>

#define ALIGNED alignas(8)

// cereal helper
#define ARCHIVE(arch, var) arch(cereal::make_nvp(#var, var))
#define ARCHIVE_NVP(arch, name, var) arch(cereal::make_nvp(#name, var))

#include "Platform.h"
#include "CommonMath.h"
#include "Enum.h"
#include "Log.h"
#include "CommonUtils.h"

// 'Type' can be 'class' or 'struct'
#define TYPED_DECLARE_PTR(Type, ClassName)             \
Type  ClassName;                                       \
using ClassName##Ptr     = std::shared_ptr<ClassName>; \
using ClassName##WeakPtr = std::weak_ptr<ClassName>;   \
using ClassName##UPtr    = std::unique_ptr<ClassName>;

#define DECLARE_PTR(ClassName) TYPED_DECLARE_PTR(class, ClassName)
#define STRUCT_DECLARE_PTR(ClassName) TYPED_DECLARE_PTR(struct, ClassName)

#define NO_COPY(Class) \
    Class(const Class&) = delete; \
    Class& operator=(const Class&) = delete;

#define NO_MOVE(Class) \
    Class(Class&&) = delete; \
    Class& operator=(Class&&) = delete;

#define NO_COPY_MOVE(Class) NO_COPY(Class) NO_MOVE(Class)

using Event = std::function<void()>;

DECLARE_PTR(Config);
class FrameContext;

DECLARE_PTR(Object);
DECLARE_PTR(Dimension);
DECLARE_PTR(Counter);

DECL_ENUM(DimensionStatus,  Active,   Pending);
DECL_ESTR(DimensionStatus, "Active", "Pending");

DECL_ENUM(LocationState,  Entered,   Placing,   Tracked,   NotTracked,   Exited)
DECL_ESTR(LocationState, "Entered", "Placing", "Tracked", "NotTracked", "Exited")

DECL_ENUM(EditableState,  Created,   Updated,   Deleted)
DECL_ESTR(EditableState, "Created", "Updated", "Deleted")

DECL_ENUM(SaveOperation,  Create,   Update,   Delete)
DECL_ESTR(SaveOperation, "Create", "Update", "Delete")

DECL_ENUM(AccessLevel,  Read,   Write,   Admin)
DECL_ESTR(AccessLevel, "Read", "Write", "Admin")

#include "ObjectId.h"

using PlacementCallback = std::function<void(size_t, const Quat& quat)>;

DECL_ENUM(FaceVertexOrder,  Clockwise,   CounterClockwise)
DECL_ESTR(FaceVertexOrder, "Clockwise", "CounterClockwise")

constexpr size_t INVALID_INDEX{static_cast<size_t>(-1)};

class Texture;
using MaterialValue = std::variant<bool, int, unsigned int, float, Vec2, Vec3, Vec4, Texture*, std::string>;

DECL_ENUM(ResourceType,  File,   Texture,   Model,   Node,   Skybox,   Material)
DECL_ESTR(ResourceType, "File", "Texture", "Model", "Node", "Skybox", "Material")

DECL_ENUM(GizmoMode,  Translate,   Rotate,   Scale)
DECL_ESTR(GizmoMode, "Translate", "Rotate", "Scale")

DECL_ENUM(AudioFormat,  Mono8,   Stereo8,  Mono16,   Stereo16,   MonoFloat32,   StereoFloat32,   MonoDouble,   StereoDouble)
DECL_ESTR(AudioFormat, "Mono8", "Stereo8" "Mono16", "Stereo16", "MonoFloat32", "StereoFloat32", "MonoDouble", "StereoDouble")

DECL_ENUM(PlaybackState,  Playing,   Paused,   Stopped,   Error)
DECL_ESTR(PlaybackState, "Playing", "Paused", "Stopped", "Error")

#endif // DIMX_CORE_COMMON_H