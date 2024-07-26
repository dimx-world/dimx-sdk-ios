#pragma once
#include <Common.h>
#include <quickjspp.hpp>
#include "JsVec3.h"
#include <sstream>

class JsRay
{
public:
    JsRay() = default;
    JsRay(const Ray& coreRay)
    : originX(coreRay.origin.x)
    , originY(coreRay.origin.y)
    , originZ(coreRay.origin.z)
    , directionX(coreRay.direction.x)
    , directionY(coreRay.direction.y)
    , directionZ(coreRay.direction.z)
    {}

    Ray core() const {
        return Ray{Vec3{originX, originY, originZ},
                   Vec3{directionX, directionY, directionZ}};
    }

    std::string str() const {
        std::ostringstream oss;
        oss << '['
            << "(" << originX << ", " << originY << ", " << originZ << "), "
            << "(" << directionX << ", " << directionY << ", " << directionZ << ')'
            << ']';
        return oss.str();
    }

    std::shared_ptr<JsVec3> origin() const {
        return std::make_shared<JsVec3>(originX, originY, originZ);
    }

    std::shared_ptr<JsVec3> direction() const {
        return std::make_shared<JsVec3>(directionX, directionY, directionZ);
    }

    double closestDistToPoint(JsVec3* point) const {
        return point ? (double)pointToRayClosestDist(core(), point->core()) : 0.0;
    }

    static void registerClass(qjs::Context::Module& module) {
        module.class_<JsRay>("Ray")
        .fun<&JsRay::str>("str")
        .fun<&JsRay::originX>("originX")
        .fun<&JsRay::originY>("originY")
        .fun<&JsRay::originZ>("originZ")
        .fun<&JsRay::directionX>("directionX")
        .fun<&JsRay::directionY>("directionY")
        .fun<&JsRay::directionZ>("directionZ")
        .fun<&JsRay::origin>("origin")
        .fun<&JsRay::direction>("direction")
        .fun<&JsRay::closestDistToPoint>("closestDistToPoint");
    }

public:
    double originX{0.0};
    double originY{0.0};
    double originZ{0.0};
    double directionX{1.0};
    double directionY{0.0};
    double directionZ{0.0};
};
