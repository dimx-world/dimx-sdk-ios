#pragma once
#include <Common.h>
#include <config/Config.h>

namespace detail {

template <typename T>
float euclideanDist(const T& a, const T& b);

template <typename T>
T interpolate(const T& a, const T& b, float fraction);

template <typename T>
std::vector<T> parseVector(const Config& values)
{
    if (!values.isArray()) {
        return {};
    }

    std::vector<T> result;
    result.reserve(values.size());

    for (const Config& val: values) {
        result.push_back(val.get<T>());
    }

    return result;
}

} // namespace detail

DECL_ENUM(ActorTrackType,  Linear,   CatmullRom,   Cubic)
DECL_ESTR(ActorTrackType, "Linear", "CatmullRom", "Cubic")

template <typename T>
class ActorTrack
{
public:
    ActorTrack(const Config& config) {
        if (config.isArray()) {
            mValues = detail::parseVector<T>(config);
        } else {
            mValues = detail::parseVector<T>(config.get<const Config&>("values", {}));
            mType = config.get("type", mType);
        }

        if (mValues.empty()) {
            return;
        }

        mPositions = detail::parseVector<float>(config.get<const Config&>("positions", {}));

        if (mPositions.size() > 0 && mPositions.size() != mValues.size()) {
            // throw
            mPositions.clear();
        }

        switch (mType) {
            case ActorTrackType::Linear: {
                bool loop = config.get("loop", false);
                if (loop) {
                    mValues.push_back(mValues.front());
                    mPositions.clear();
                }
                break;
            }
            case ActorTrackType::CatmullRom: {
                int pointsPerSegment = config.get("points_per_segment", 4);
                bool loop = config.get("loop", false);
                mValues = calcCatmullRomCurve(mValues, pointsPerSegment, loop);
                mPositions.clear();
                break;
            }
            case ActorTrackType::Cubic: {
                int pointsPerSegment = config.get("points_per_segment", 4);
                bool loop = config.get("loop", false);
                mValues = calcCubicCurve(mValues, pointsPerSegment, loop);
                mPositions.clear();
                break;
            }
            default: break;
        }

        std::vector<float> eucPositions(mValues.size());
        for (size_t i = 1; i < mValues.size(); ++i) {
            mEuclideanLength += detail::euclideanDist(mValues[i - 1], mValues[i]);
            eucPositions[i] = mEuclideanLength;
        }

        if (mPositions.empty()) {
            mPositions = std::move(eucPositions);
            if (mEuclideanLength > 0) {
                for (auto& p: mPositions) {
                    p /= mEuclideanLength;
                }
            }
        }
    }

    size_t numPoints() const { mValues.size(); }
    float euclideanLength() const { return mEuclideanLength; }

    T evaluate(float position) {
        if (mValues.size() < 2) {
            return !mValues.empty() ? mValues[0] : T{};
        }

        if (position < -1.f || position > 1.f) {
            position = position - std::trunc(position);
        }

        if (position < 0.f) {
            position = 1.f - position;
        }

        if (position < mPositions[mLastIdx]) {
            mLastIdx = 0;
        }

        while (mLastIdx + 1 < mPositions.size() && mPositions[mLastIdx + 1] < position) {
            ++mLastIdx;
        }

        float frac = (position - mPositions[mLastIdx]) / (mPositions[mLastIdx+1] - mPositions[mLastIdx]);
        return detail::interpolate(mValues[mLastIdx], mValues[mLastIdx + 1], frac);
    }

private:
    std::vector<T> calcCatmullRomCurve(const std::vector<T>& controlPoints, int pointsPerSegment, bool loop) {
        std::vector<T> smoothPath;

        if (controlPoints.size() < (loop ? 4 : 3)) {
            // Not enough points to create a smooth path
            return smoothPath;
        }

        size_t numSegments = loop ? controlPoints.size() : controlPoints.size() - 1;

        for (size_t i = 0; i < numSegments; ++i) {
            // Calculate indices, wrapping around if looping
            size_t index0 = (i + controlPoints.size() - 1) % controlPoints.size();
            size_t index1 = i % controlPoints.size();
            size_t index2 = (i + 1) % controlPoints.size();
            size_t index3 = (i + 2) % controlPoints.size();

            const T& p0 = controlPoints[index0];
            const T& p1 = controlPoints[index1];
            const T& p2 = controlPoints[index2];
            const T& p3 = controlPoints[(loop || i + 2 < controlPoints.size()) ? index3 : index2]; // Avoid going out of bounds if not looping

            for (int j = 0; j < pointsPerSegment; ++j) {
                float t = j / (float)pointsPerSegment;
                const T& point = calcCatmullRom(p0, p1, p2, p3, t);
                smoothPath.push_back(point);
            }
        }

        // Add the last segment if not looping
        if (!loop) {
            const T& lastSegment = calcCatmullRom(
                controlPoints[controlPoints.size() - 3],
                controlPoints[controlPoints.size() - 2],
                controlPoints[controlPoints.size() - 1],
                controlPoints[controlPoints.size() - 1], // Repeat the last point
                0.5f); // Middle point for the last segment
            smoothPath.push_back(lastSegment);
        } else {
            // Optionally, close the loop explicitly by adding the first point at the end
            smoothPath.push_back(smoothPath.front());
        }

        return smoothPath;
    }

    std::vector<T> calcCubicCurve(const std::vector<T>& controlPoints, int pointsPerSegment, bool loop) {
        std::vector<T> smoothPath;

        if (controlPoints.size() < (loop ? 4 : 3)) {
            // Not enough points to create a smooth path
            return smoothPath;
        }

        size_t numSegments = loop ? controlPoints.size() : controlPoints.size() - 1;

        for (size_t i = 0; i < numSegments; ++i) {
            // Calculate indices, wrapping around if looping
            size_t index0 = (i + controlPoints.size() - 1) % controlPoints.size();
            size_t index1 = i % controlPoints.size();
            size_t index2 = (i + 1) % controlPoints.size();
            size_t index3 = (i + 2) % controlPoints.size();

            const T& p0 = controlPoints[index0];
            const T& p1 = controlPoints[index1];
            const T& p2 = controlPoints[index2];
            const T& p3 = controlPoints[(loop || i + 2 < controlPoints.size()) ? index3 : index2]; // Avoid going out of bounds if not looping

            for (int j = 0; j < pointsPerSegment; ++j) {
                float t = j / (float)pointsPerSegment;
                const T& point = calcCubic(p0, p1, p2, p3, t);
                smoothPath.push_back(point);
            }
        }

        // Add the last segment if not looping
        if (!loop) {
            const T& lastSegment = calcCubic(
                controlPoints[controlPoints.size() - 3],
                controlPoints[controlPoints.size() - 2],
                controlPoints[controlPoints.size() - 1],
                controlPoints[controlPoints.size() - 1], // Repeat the last point
                0.5f); // Middle point for the last segment
            smoothPath.push_back(lastSegment);
        } else {
            // Optionally, close the loop explicitly by adding the first point at the end
            smoothPath.push_back(smoothPath.front());
        }

        return smoothPath;
    }

private:
    ActorTrackType mType{ActorTrackType::Linear};
    std::vector<T> mValues;
    std::vector<float> mPositions;

    float mEuclideanLength{0.f};

    size_t mLastIdx{0};
};
