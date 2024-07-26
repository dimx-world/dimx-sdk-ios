#pragma once

#ifndef DIMX_CORE_BINARY_DATA_H
#define DIMX_CORE_BINARY_DATA_H

#include <Buffer.h>
#include <CommonMath.h>
#include <Transform.h>
#include <BinaryData.h>
#include <anim/AnimTrack.h>

#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/memory.hpp>

#include <string>
#include <vector>
#include <map>
#include <memory>

DECLARE_PTR(BinaryData);
class BinaryData
{
public:
    struct MorphTarget {
        std::string name;
        std::vector<uint32_t> vertIds;
        std::vector<Vec3> positions;
        std::vector<Vec3> normals;
        std::vector<Vec3> tangents;
        
        template <class Archive> void serialize(Archive & ar) {
            ARCHIVE(ar, name);
            ARCHIVE(ar, vertIds);
            ARCHIVE(ar, positions);
            ARCHIVE(ar, normals);
            ARCHIVE(ar, tangents);
        }
    };

    struct Geometry {
        std::vector<Vec3> positions;
        std::vector<Vec3> normals;
        std::vector<Vec3> tangents;
        std::vector<Vec3> bitangents;
        std::vector<Vec2> texCoords;
        std::vector<Vec4> colors;
        std::vector<float> jointIndices;
        std::vector<Vec4> jointIndices4;
        std::vector<Vec4> jointWeights4;
        std::vector<uint16_t> inds16;
        std::vector<uint32_t> inds32;

        std::vector<MorphTarget> morphTargets;

        uint8_t material = 0;

        AABB aabb;

        template <class Archive> void serialize(Archive & ar) {
            ARCHIVE(ar, positions);
            ARCHIVE(ar, normals);
            ARCHIVE(ar, tangents);
            ARCHIVE(ar, bitangents);
            ARCHIVE(ar, texCoords);
            ARCHIVE(ar, colors);
            ARCHIVE(ar, jointIndices);
            ARCHIVE(ar, jointIndices4);
            ARCHIVE(ar, jointWeights4);
            ARCHIVE(ar, inds16);
            ARCHIVE(ar, inds32);
            ARCHIVE(ar, morphTargets);
            ARCHIVE(ar, material);
            ARCHIVE(ar, aabb);
        }
    };

    struct Mesh {
        int32_t skeleton = -1;
        std::vector<Mat4> jointInvBindPoses;
        std::vector<Geometry> chunks;

        template <class Archive> void serialize(Archive & ar) {
            ARCHIVE(ar, skeleton);
            ARCHIVE(ar, jointInvBindPoses);
            ARCHIVE(ar, chunks);
        }
    };

    struct Node {
        std::string name;
        int32_t parent = -1;
        RawTransform transform;
        int32_t mesh = -1;

        template <class Archive> void serialize(Archive & ar) {
            ARCHIVE(ar, name);
            ARCHIVE(ar, parent);
            ARCHIVE(ar, transform);
            ARCHIVE(ar, mesh);
        }
    };

    struct Animation {
        std::string name;
        double startTime = 0.0;
        double endTime = 0.0;
        double frameRate = 0.0;
        uint32_t numFrames = 0;

        std::vector<AnimTransformNode> transformNodes;
        std::vector<TransformTrack> transformTracks;

        std::vector<uint32_t> skelNodes;
        Buffer skelAnimation;

        std::vector<AnimMorphNode> morphNodes;
        std::vector<FloatTrack> floatTracks;

        template <class Archive> void serialize(Archive & ar) {
            ARCHIVE(ar, name);
            ARCHIVE(ar, startTime);
            ARCHIVE(ar, endTime);
            ARCHIVE(ar, frameRate);
            ARCHIVE(ar, numFrames);
            ARCHIVE(ar, transformNodes);
            ARCHIVE(ar, transformTracks);
            ARCHIVE(ar, skelNodes);
            ARCHIVE(ar, skelAnimation);
            ARCHIVE(ar, morphNodes);
            ARCHIVE(ar, floatTracks);
        }
    };

    struct Material {
        std::string name;
        std::map<std::string, std::string> properties;
        
        template <class Archive> void serialize(Archive & ar) {
            ARCHIVE(ar, name);
            ARCHIVE(ar, properties);
        }
    };

    struct Skeleton {
        Buffer ozzSkeleton;
        template <class Archive> void serialize(Archive & ar) {
            ARCHIVE(ar, ozzSkeleton);
        }
    };

public:
    template <class Archive> void serialize(Archive & ar, std::uint32_t version) {
        ARCHIVE(ar, nodes);
        ARCHIVE(ar, meshes);
        ARCHIVE(ar, materials);
        ARCHIVE(ar, animations);
        ARCHIVE(ar, skeletons);
    }

    void loadFromBuffer(const Buffer& buffer);
    void saveToBuffer(Buffer& buffer);

    std::string summaryInfo() const;

public:
    std::vector<Node> nodes;
    std::vector<Mesh> meshes;
    std::vector<Material> materials;
    std::vector<Animation> animations;
    std::vector<Skeleton> skeletons;

private:
    //-----------------------------
    // Do not serialize!
    size_t mCompressedSize = 0;
    size_t mDecompressedSize = 0;
    //-----------------------------
};

CEREAL_CLASS_VERSION(::BinaryData, 0)

#endif // DIMX_CORE_BINARY_DATA_H