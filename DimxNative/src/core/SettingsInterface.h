#ifndef SETTINGS_INTERFACE_H_INCLUDED
#define SETTINGS_INTERFACE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

bool Settings_displayOcclusionObjects();
bool Settings_displayMarkers();
bool Settings_displayDebugLines();
bool Settings_displayPointCloud();
bool Settings_displayDebugPlanes();
bool Settings_displayShadows();
bool Settings_debugRender();
long Settings_iosRaycastStrategy();
bool Settings_iosPlaneDetection();

#ifdef __cplusplus
}
#endif

#endif // SETTINGS_INTERFACE_H_INCLUDED
