#ifndef ANALYTICS_MANAGER_INTERFACE_H_INCLUDED
#define ANALYTICS_MANAGER_INTERFACE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

void AnalyticsManager_setOSVersion(const char* value);
void AnalyticsManager_setDeviceModel(const char* value);

#ifdef __cplusplus
}
#endif

#endif // ANALYTICS_MANAGER_INTERFACE_H_INCLUDED
