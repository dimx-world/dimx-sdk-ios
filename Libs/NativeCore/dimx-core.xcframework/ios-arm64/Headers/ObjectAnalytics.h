#pragma once

#ifndef DIMX_CORE_OBJECT_ANALYTICS_H
#define DIMX_CORE_OBJECT_ANALYTICS_H

#include <string>

class Object;
class ObjectAnalytics
{
public:
    ObjectAnalytics(Object* entity);
    ~ObjectAnalytics();
    void onUpdate();

private:
    void logEvent(const std::string& event);

private:
    Object* mEntity{nullptr};
    bool mUpdated{false};
};

#endif // DIMX_CORE_OBJECT_ANALYTICS_H