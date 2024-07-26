#include "SettingsInterface.h"
#include <Engine.h>

bool Settings_displayOcclusionObjects()
{
    return g_settings().displayOcclusionObjects();
}

bool Settings_displayMarkers()
{
    return g_settings().displayMarkers();
}

bool Settings_displayDebugLines()
{
    return g_settings().displayDebugLines();
}

bool Settings_displayPointCloud()
{
    return g_settings().displayPointCloud();
}

bool Settings_displayDebugPlanes()
{
    return g_settings().displayDebugPlanes();
}

bool Settings_displayShadows()
{
    return g_settings().displayShadows();
}

bool Settings_debugRender()
{
    return g_settings().debugRender();
}

long Settings_iosRaycastStrategy()
{
    return static_cast<long>(g_settings().iosRaycastStrategy());
}

bool Settings_iosPlaneDetection()
{
    return g_settings().iosPlaneDetection();
}