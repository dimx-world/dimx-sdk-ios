#pragma once

#ifndef DIMX_CORE_GEOLOCATION_MANAGER_H
#define DIMX_CORE_GEOLOCATION_MANAGER_H

#include <Common.h>
#include <config/Config.h>

struct GeoCoords {
    GeoCoords() = default;
    GeoCoords(double _lat, double _lng, double _alt, double _hacc, double _vacc)
    : lat(_lat), lng(_lng), alt(_alt), hacc(_hacc), vacc(_vacc)
    {}

    double lat{.0};
    double lng{.0};
    double alt{.0};
    double hacc{-1.};
    double vacc{-1.};
};

class GeolocationManager
{
    using Callback = std::function<void(const GeoCoords&)>;

public:
    void initialize(const Config& config);
    void deinitialize();
    void onDeviceLocationUpdate(const GeoCoords& coords);
    void subscribeOnUpdate(Callback callback);
    bool hasValidPosition() const { return mUpdateTime >= 0.0; }

    const GeoCoords& coords() const { return mCoords; }

    void forceUpdate();

private:
    double mSignificantDist{2.0};
    GeoCoords mCoords;
    double mUpdateTime{-1.0};
    std::vector<Callback> mCallbacks;
    GeoCoords mLastCallbackCoords;
};

#endif // DIMX_CORE_GEOLOCATION_MANAGER_H