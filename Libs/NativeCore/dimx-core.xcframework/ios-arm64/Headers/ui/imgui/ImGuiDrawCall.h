
#ifndef IM_GUI_DRAW_CALL_H_INCLUDED
#define IM_GUI_DRAW_CALL_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

struct ImGuiDrawCall {
    long bufferId;
    const void* vertexData;
    long numVertsInBuffer;
    const void* indexData;
    long numIndsInBuffer;
    int clip_x;
    int clip_y;
    int clip_width;
    int clip_height;
    void* texId;
    long numIndsInCall;
    long indOffsetInCall;
};

typedef struct ImGuiDrawCall ImGuiDrawCall;

#ifdef __cplusplus
}
#endif

#endif // IM_GUI_DRAW_CALL_H_INCLUDED
