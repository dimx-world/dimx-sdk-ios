#pragma once
#include <Common.h>

struct RawTransform
{
    Vec3 position;
    Quat rotation;
    Vec3 scale;

    template <class Archive> void serialize(Archive & ar) {
        ARCHIVE(ar, position);
        ARCHIVE(ar, rotation);
        ARCHIVE(ar, scale);
    }
};

/*
struct TransformTrack {
    enum {TX = 0, TY, TZ, QX, QY, QZ, QW, SX, SY, SZ, NUM_TRACKS};
    std::string name;
    double startTime = 0.0;
    double endTime = 0.0;
    std::array<Track, NUM_TRACKS> tracks;

    template <class Archive> void serialize(Archive & ar) {
        ARCHIVE(ar, name);
        ARCHIVE(ar, startTime);
        ARCHIVE(ar, endTime);
        ARCHIVE(ar, tracks);
    }
};
*/

struct TransformTrack
{
    std::vector<RawTransform> keys;
    template <class Archive> void serialize(Archive & ar) {
        ARCHIVE(ar, keys);
    }
};

struct FloatTrack
{
    std::vector<float> keys;
    template <class Archive> void serialize(Archive & ar) {
        ARCHIVE(ar, keys);
    }
};

struct AnimTransformNode {
    uint32_t node = -1;
    uint32_t track = -1;
    template <class Archive> void serialize(Archive & ar) {
        ARCHIVE(ar, node);
        ARCHIVE(ar, track);
    }
};

struct AnimMorphTarget {
    uint32_t target = -1;
    uint32_t track = -1;
    template <class Archive> void serialize(Archive & ar) {
        ARCHIVE(ar, target);
        ARCHIVE(ar, track);
    }
};

struct AnimMorphNode {
    uint32_t node = -1;
    std::vector<AnimMorphTarget> targets;
    template <class Archive> void serialize(Archive & ar) {
        ARCHIVE(ar, node);
        ARCHIVE(ar, targets);
    }
};

