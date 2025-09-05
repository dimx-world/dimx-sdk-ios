#pragma once
#include <Common.h>
#include <quickjspp.hpp>

class JsVec3
{
public:
    JsVec3(double _x = 0.0, double _y = 0.0, double _z = 0.0) 
    : x(_x), y(_y), z(_z)
    {}

    JsVec3(const Vec3& coreVec)
    : x(coreVec.x)
    , y(coreVec.y)
    , z(coreVec.z)
    {}

    Vec3 core() const {
        return Vec3{x, y, z};
    }

    std::string str() const {
        std::ostringstream oss;
        oss << x << ' ' << y << ' ' << z;
        return oss.str();
    }

    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }

    double len() const {
        return static_cast<double>(length(core()));
    }

    double lenSq() const {
        return static_cast<double>(lengthSq(core()));
    }

    std::shared_ptr<JsVec3> add(JsVec3* other) const {
        if (!other) { return nullptr; }
        return std::make_shared<JsVec3>(core() + other->core());
    }

    std::shared_ptr<JsVec3> sub(JsVec3* other) const {
        if (!other) { return nullptr; }
        return std::make_shared<JsVec3>(core() - other->core());
    }

    std::shared_ptr<JsVec3> cross(JsVec3* other) const {
        if (!other) { return nullptr; }
        return std::make_shared<JsVec3>(::cross(core(), other->core()));
    }

    double dot(JsVec3* other) const {
        if (!other) { return 0; }
        return ::dot(core(), other->core());
    }

    std::shared_ptr<JsVec3> mult(double v) const {
        return std::make_shared<JsVec3>(core() * (float)v);
    }

    static void registerClass(qjs::Context::Module& module) {
        module.class_<JsVec3>("Vec3")
        .constructor<double, double, double>()
        .fun<&JsVec3::str>("str")
        .fun<&JsVec3::x>("x")
        .fun<&JsVec3::y>("y")
        .fun<&JsVec3::z>("z")
        .fun<&JsVec3::getX>("getX")
        .fun<&JsVec3::getY>("getY")
        .fun<&JsVec3::getZ>("getZ")
        .fun<&JsVec3::len>("len")
        .fun<&JsVec3::lenSq>("lenSq")
        .fun<&JsVec3::add>("add")
        .fun<&JsVec3::sub>("sub")
        .fun<&JsVec3::cross>("cross")
        .fun<&JsVec3::dot>("dot")
        .fun<&JsVec3::mult>("mult");
    }

public:
    double x{0.0};
    double y{0.0};
    double z{0.0};
};
