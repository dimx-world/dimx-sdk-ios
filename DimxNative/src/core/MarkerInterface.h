#ifndef MARKER_INTERFACE_H_INCLUDED
#define MARKER_INTERFACE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

void TrackingResult_assign(void* ptr, bool tracked, const void* const transform);

const char* Marker_name(const void* ptr);
const void* Marker_image(const void* ptr);
float Marker_width(const void* ptr);
float Marker_height(const void* ptr);

#ifdef __cplusplus
}
#endif

#endif // MARKER_INTERFACE_H_INCLUDED