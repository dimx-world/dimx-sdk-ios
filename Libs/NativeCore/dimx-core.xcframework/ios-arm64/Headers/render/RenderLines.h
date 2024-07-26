#pragma once
#include "Common.h"

struct DebugVertex {
    float x, y, z, r, g, b, a;
};

class RenderLines
{
    NO_COPY_MOVE(RenderLines)

public:
    RenderLines() = default;
    void reset();

    void drawPoint(const Vec3& pos, float worldSize, const Vec4& col);
    void drawLine(const Vec3& pos1, const Vec3& pos2, const Vec4& col);
    void drawLine(const Vec3& pos1, const Vec4& col1, const Vec3& pos2, const Vec4& col2);
    void drawLineRect(float width, float height, const Mat4& transform, const Vec4& col);
    void drawLineBox(const Vec3& min, const Vec3& max, const Mat4& transform, const Vec4& col);
    void drawSolidBox(const Vec3& min, const Vec3& max, const Mat4& transform, const Vec4& col);
    void drawPolygon(const std::vector<Vec3>& verts, const Vec4& col);
    void drawPose(const Mat4& transform, float size);
    void drawCircleXZ(const Vec3& pos, float radius, const Vec4& col);
    void drawSphere(const Vec3& pos, float radius, const Vec4& col);

    const std::vector<DebugVertex>& lines() const;
    const std::vector<DebugVertex>& polygons() const;

private:
    void drawLine(const Vec4& pos1, const Vec4& pos2, const Vec4& col);

private:
    std::vector<DebugVertex> mLines;
    std::vector<DebugVertex> mPolygons;
};
