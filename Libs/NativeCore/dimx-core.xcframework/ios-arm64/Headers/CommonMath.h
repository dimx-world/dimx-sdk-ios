#pragma once

#ifndef DIMX_CORE_COMMON_MATH_H
#define DIMX_CORE_COMMON_MATH_H

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_CTOR_INIT // IMPORTANT: default initialization to Identity()

#ifdef DIMX_PLATFORM_IOS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#endif

#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/gtx/spline.hpp>
#include <glm/gtx/matrix_decompose.hpp>

constexpr float PI = glm::pi<float>();
constexpr float TWO_PI = PI * 2.f;
constexpr float DEG_TO_RAD = PI / 180.f;
constexpr float RAD_TO_DEG = 180.f / PI;
constexpr double EARTH_RADIUS = 6371000.0; // radius of the Earth in meters

using Vec2   = glm::vec2;
using Vec2i  = glm::ivec2;
using Vec3   = glm::vec3;
using Vec3i  = glm::ivec3;
using Vec4   = glm::vec4;
using Vec4i  = glm::ivec4;
using Vec4ub = glm::i8vec4;

using Mat4 = glm::mat4;
using Mat3 = glm::mat3;

// Note on components order!
// Constructor: Quat(w, x, y, z)
// Internally: struct {T x, y, z, w;}
using Quat = glm::quat;

struct Ray {
    Ray() = default;
    Ray(const Vec3& orig, Vec3 dir): origin(orig), direction(dir) {}
    bool valid() const { return direction.x != 0.f && direction.y != 0.f && direction.z != 0.f; };
    Vec3 pointAtDist(float dist) const { return origin + direction * dist; }
    Vec3 origin;
    Vec3 direction;
};

// Axis Aligned Bounding Box
struct AABB {
    Vec3 min;
    Vec3 max;
    void extend(const Vec3& point) {
        min.x = std::min(min.x, point.x);
        min.y = std::min(min.y, point.y);
        min.z = std::min(min.z, point.z);
        max.x = std::max(max.x, point.x);
        max.y = std::max(max.y, point.y);
        max.z = std::max(max.z, point.z);
    }
    void extend(const AABB& other) {
        extend(other.min);
        extend(other.max);
    }

    bool empty() const {
        return min == max;
    }

    Vec3 center() const { return (min + max) * 0.5f; }
};

struct RaycastResult {
    bool valid = false;
    Vec3 position{0.f, 0.f, 0.f};
    Quat rotation{};
    Vec3 normal{0.f, 0.f, 0.f};
    void* data = nullptr;
};

// Returns true for 0
inline bool isPowerOfTwo(size_t x) { return (x & (x - 1)) == 0; }

inline bool intersectRayPlane(const Vec3& rayOrig,
                              const Vec3& rayDir,
                              const Vec3& planeOrig,
                              const Vec3& planeNormal,
                              float& intersectionDistance)
{
    return glm::intersectRayPlane(rayOrig, rayDir, planeOrig, planeNormal, intersectionDistance);
}

inline float length(const Vec3& vec) { return glm::length(vec); }
inline float lengthSq(const Vec3& vec) { return vec.x*vec.x + vec.y*vec.y + vec.z*vec.z; }
inline Vec3 normalize(Vec3& vec) { return glm::normalize(vec); }
inline Vec4 normalize(Vec4& vec) { return glm::normalize(vec); }
inline Quat normalize(Quat& quat) { return glm::normalize(quat); }

inline Vec3 cross(const Vec3& v1, const Vec3& v2) { return glm::cross(v1, v2); }
inline float dot(const Vec3& v1, const Vec3& v2) { return glm::dot(v1, v2); }
inline float dot(const Quat& q1, const Quat& q2) { return glm::dot(q1, q2); }

inline Vec3 closestPointOnRay(const Ray& ray, const Vec3& p) {
    //return glm::closestPointOnLine(p, ray.origin, ray.origin + ray.direction * 100.f);
    float dist = dot(p - ray.origin, ray.direction);
    return dist > 0.f ? ray.origin + ray.direction * dist : ray.origin;
}

inline float pointToRayClosestDist(const Ray& ray, const Vec3& p) {
    return glm::distance(closestPointOnRay(ray, p), p);
}

inline Mat4 inverse(const Mat4& mat) { return glm::inverse(mat); }
inline Quat inverse(const Quat& quat) { return glm::inverse(quat); }

inline float deg2rad(const float deg) {return deg * DEG_TO_RAD;}
inline float rad2deg(const float rad) {return rad * RAD_TO_DEG;}

inline Mat3 model2normalMat(const Mat4& modelMat)
{
    return glm::transpose(glm::inverse(glm::mat3(modelMat)));
}

inline Mat4 ortho(float left, float  right, float bottom, float top)
{
    return glm::ortho(left, right, bottom, top);
}

inline Mat4 ortho(float left, float  right, float bottom, float top, float near, float far)
{
    return glm::ortho(left, right, bottom, top, near, far);
}

inline Mat4 lookAt(const Vec3& pos, const Vec3& target, const Vec3& up)
{
    return glm::lookAt(pos, target, up);
}

inline Mat4 perspective(float fov, float aspect, float near, float far)
{
    return glm::perspective(fov, aspect, near, far);
}

inline Quat quatLookAt(const Vec3& dir, const Vec3& up)
{
    return glm::quatLookAt(dir, up);
}

inline Quat quatLerp(const Quat& q1, const Quat& q2, float t) {
    return glm::lerp(q1, q2, t);
}

inline Quat quatSlerp(const Quat& q1, const Quat& q2, float t) {
    return glm::lerp(q1, q2, t);
}

inline Mat4 quat2mat(const Quat& quat)
{
    return glm::toMat4(quat);
}

inline Quat mat2Quat(const Mat4& mat)
{
    return glm::toQuat(mat);
}

inline Quat angles2quat(const Vec3& eulerAngles)
{
    return Quat(eulerAngles);
}

inline Vec3 quat2angles(const Quat& quat)
{
    return glm::eulerAngles(quat);
}

inline Quat angleAxis(float angle, const Vec3& axis)
{
    return glm::angleAxis(angle, axis);
}

inline Vec3 translation(const Mat4& mat)
{
    return Vec3(mat[3]);
}

inline Mat4 translate(const Mat4& mat, const Vec3& vec)
{
    return glm::translate(mat, vec);
}

inline Quat rotation(const Mat4& mat)
{
    return Quat(mat);
}

inline Mat4 rotate(const Mat4& mat, float angle, const Vec3& axis)
{
    return glm::rotate(mat, angle, axis);
}

inline Mat4 scale(float sx, float sy, float sz)
{
    return glm::scale(Mat4(), glm::vec3(sx, sy, sz));
}

inline bool decomposeMatrix(const Mat4& mat, Vec3& scale, Quat& quat, Vec3& translate)
{
    Vec3 skew;
    Vec4 persp;
    return glm::decompose(mat, scale, quat, translate, skew, persp);
}

float intersectRayAABB(const Ray& ray, const AABB& aabb);

double calcGPSDistance(double lat1, double lon1, double lat2, double lon2);
bool checkGPSBounds(double centerLat, double centerLng, double maxDistance, double pointLat, double pointLng);

template<typename T,
         typename std::enable_if<!std::is_same<T, float>::value, bool>::type = true>
T calcCatmullRom(T const& v1, T const& v2, T const& v3, T const& v4, typename T::value_type const& s) {
    return glm::catmullRom<T>(v1, v2, v3, v4, s);
}

inline float calcCatmullRom(float v1, float v2, float v3, float v4, float s) {
    return 0.5f * ((2.0f * v2) +
                   (-v1 + v3) * s +
                   (2.0f * v1 - 5.0f * v2 + 4.0f * v3 - v4) * s * s +
                   (-v1 + 3.0f * v2 - 3.0f * v3 + v4) * s * s * s);
}

template<typename T,
         typename std::enable_if<!std::is_same<T, float>::value, bool>::type = true>
T calcCubic(T const& v1, T const& v2, T const& v3, T const& v4, typename T::value_type const& s) {
    return glm::cubic<T>(v1, v2, v3, v4, s);
}

inline float calcCubic(float v1, float v2, float v3, float v4, float s) {
    return ((v1 * s + v2) * s + v3) * s + v4;
}
//------------------------------------------------------------------------------

#include <ostream>
inline std::ostream& operator << (std::ostream& os, const Vec2& vec) {
    return os << '[' << vec.x << ", " << vec.y << ']';
}

inline std::ostream& operator << (std::ostream& os, const Vec3& vec) {
    return os << '[' << vec.x << ", " << vec.y << ", " << vec.z << ']';
}

inline std::ostream& operator << (std::ostream& os, const Vec4& vec) {
    return os << '[' << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ']';
}

inline std::ostream& operator << (std::ostream& os, const Quat& quat) {
    return os << '[' << quat[0] << ", " << quat[1] << ", " << quat[2] << ", " << quat[3] << ']';
}

inline std::ostream& operator << (std::ostream& os, const Mat4& mat) {
    os << '[';
    for (int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            os << mat[i][j];
            if (j < 3) os << ", ";
        }
        if (i < 3) os << "| ";
    }
    os << ']';

    return os;
}

inline std::ostream& operator << (std::ostream& os, const AABB& aabb) {
    return os << '[' << aabb.min << ' ' << aabb.max << ']';
}

inline std::ostream& operator << (std::ostream& os, const RaycastResult& res) {
    return os << '[' << res.valid << ' ' << res.position << ' ' << res.normal << ']';
}

//------------------------------------------------------------------------------

#include "CerealUtils.hpp"

namespace cereal {

// cereal/macros.hpp:
// #define CEREAL_SERIALIZE_FUNCTION_NAME serialize

template <class Archive> inline
void serialize(Archive & ar, Vec2 const & a)
{
    ARCHIVE_NVP(ar, x, a.x);
    ARCHIVE_NVP(ar, y, a.y);
}

template <class Archive> inline
void serialize(Archive & ar, Vec2 & a)
{
    ARCHIVE_NVP(ar, x, a.x);
    ARCHIVE_NVP(ar, y, a.y);
}

template <class Archive> inline
void serialize(Archive & ar, Vec3 const & a)
{
    ARCHIVE_NVP(ar, x, a.x);
    ARCHIVE_NVP(ar, y, a.y);
    ARCHIVE_NVP(ar, z, a.z);
}

template <class Archive> inline
void serialize(Archive & ar, Vec3 & a)
{
    ARCHIVE_NVP(ar, x, a.x);
    ARCHIVE_NVP(ar, y, a.y);
    ARCHIVE_NVP(ar, z, a.z);
}

template <class Archive> inline
void serialize(Archive & ar, Vec4 const & a)
{
    ARCHIVE_NVP(ar, x, a.x);
    ARCHIVE_NVP(ar, y, a.y);
    ARCHIVE_NVP(ar, z, a.z);
    ARCHIVE_NVP(ar, w, a.w);
}

template <class Archive> inline
void serialize(Archive & ar, Vec4 & a)
{
    ARCHIVE_NVP(ar, x, a.x);
    ARCHIVE_NVP(ar, y, a.y);
    ARCHIVE_NVP(ar, z, a.z);
    ARCHIVE_NVP(ar, w, a.w);
}

template <class Archive> inline
void serialize(Archive & ar, Vec4i const & a)
{
    ARCHIVE_NVP(ar, x, a.x);
    ARCHIVE_NVP(ar, y, a.y);
    ARCHIVE_NVP(ar, z, a.z);
    ARCHIVE_NVP(ar, w, a.w);
}

template <class Archive> inline
void serialize(Archive & ar, Vec4i & a)
{
    ARCHIVE_NVP(ar, x, a.x);
    ARCHIVE_NVP(ar, y, a.y);
    ARCHIVE_NVP(ar, z, a.z);
    ARCHIVE_NVP(ar, w, a.w);
}

template <class Archive> inline
void serialize(Archive & ar, Quat const & a)
{
    ARCHIVE_NVP(ar, x, a.x);
    ARCHIVE_NVP(ar, y, a.y);
    ARCHIVE_NVP(ar, z, a.z);
    ARCHIVE_NVP(ar, w, a.w);
}

template <class Archive> inline
void serialize(Archive & ar, Quat & a)
{
    ARCHIVE_NVP(ar, x, a.x);
    ARCHIVE_NVP(ar, y, a.y);
    ARCHIVE_NVP(ar, z, a.z);
    ARCHIVE_NVP(ar, w, a.w);
}

template <class Archive> inline
void serialize(Archive & ar, Ray const & a)
{
    ARCHIVE_NVP(ar, origin, a.origin);
    ARCHIVE_NVP(ar, direction, a.direction);
}

template <class Archive> inline
void serialize(Archive & ar, Ray & a)
{
    ARCHIVE_NVP(ar, origin, a.origin);
    ARCHIVE_NVP(ar, direction, a.direction);
}

template <class Archive> inline
void serialize(Archive & ar, AABB const & a)
{
    ARCHIVE_NVP(ar, min, a.min);
    ARCHIVE_NVP(ar, max, a.max);
}

template <class Archive> inline
void serialize(Archive & ar, AABB & a)
{
    ARCHIVE_NVP(ar, min, a.min);
    ARCHIVE_NVP(ar, max, a.max);
}

template <class Archive> inline
void serialize(Archive & ar, Mat4 & a)
{
    ARCHIVE_NVP(ar, 0, a[0]);
    ARCHIVE_NVP(ar, 1, a[1]);
    ARCHIVE_NVP(ar, 2, a[2]);
    ARCHIVE_NVP(ar, 3, a[3]);
}

} // namespace cereal

#endif // DIMX_CORE_COMMON_MATH_H