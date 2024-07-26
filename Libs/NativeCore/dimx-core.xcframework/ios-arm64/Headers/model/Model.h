#pragma once
#include <ecs/Component.h>

#include <EventPublisher.h>
#include <anim/Animation.h>
#include <anim/Skeleton.h>
#include <Counter.h>

#include <vector>
#include <string>

DECL_ENUM(ModelEvent,  AddAnimation,   Rebuild)
DECL_ESTR(ModelEvent, "AddAnimation", "Rebuild")

class Mesh;
class ModelBuilder;
class Model: public Component
{
    DECLARE_COMPONENT(Model)
    DECLARE_EVENTS(ModelEvent)
public:
    struct SkeletonInfo {
        SkeletonPtr skel;
    };

    struct SkinInfo {
        uint32_t skelId = -1;
        std::vector<Mat4> jointInvBindPoses;
    };

    struct MeshInfo {
        std::vector<std::shared_ptr<Mesh>> chunks;
        std::shared_ptr<SkinInfo> skin;
        AABB aabb;
    };

    struct NodeInfo {
        int32_t parent = -1;
        std::string name;
        bool visible{true};
        Transform transform;
        std::shared_ptr<MeshInfo> mesh;
        int32_t meshId = -1;
    };

    struct FileItem {
        std::string name;
        MessageTag msg{MessageTag::None};
        std::vector<std::string> anims;
    };

public:
    static const std::vector<std::string>& validFileExts();
    static bool isModelFbxFile(const std::string& file);
    static bool isModelFbxAnimFile(const std::string& file);
    static bool isModelBinFile(const std::string& file);
    static bool isModelBinAnimFile(const std::string& file);
    static std::string fbxToBinPath(const std::string& path);
    static std::string makeModelBinFile(const std::string& modelName);
    static std::string makeFilesPrefix(const std::string& modelName);
    static std::string makeFileFullPath(const std::string& modelName, const std::string& path);
    static std::string makeShortName(const std::string& modelFullName);
    static std::string findMainBinFile(const std::vector<std::string>& files);

    static ConfigPtr makeConfig(ObjectId id, const Config* baseConfig, const std::vector<std::string>& files);
    static void addErrorToConfig(Config& config, const std::string& error);

    Model(Object* entity, const Config& config);

    void initialize(CounterPtr counter) override;
    void serialize(Config& out) override;

    const std::unique_ptr<ModelBuilder>& builder() const;

    const std::vector<NodeInfo>& nodes() const;
    NodeInfo* getNode(const std::string& name);
    NodeInfo* getOrCreateNode(const std::string& name);
    
    void addPendingFile(const std::string& filename);
    void addFileByName(const std::string& filename);
    void addAnimFile(ObjectPtr fileObj);
    void deleteFile(const std::string& filename);

    const std::map<std::string, AnimationPtr>& animations() const;
    AnimationPtr findAnimation(const std::string& name);

    const std::vector<SkeletonInfo>& skeletons() const;

    const AABB& aabb() const { return mAABB; }
    void setAABB(const AABB& aabb) { mAABB = aabb; }

    const std::string& getDetails();
    const std::string& getMaterialDetails();

    const std::map<std::string, FileItem>& files() const { return mFiles; }

    void recordError(const std::string& error) { mErrors.push_back(error); }
    const std::vector<std::string>& errors() const { return mErrors; }

    const ObjectPtr& thumbnailTex() const { return mThumbnailTex; }

    float defaultScale() const { return mDefaultScale; }
protected:
    std::shared_ptr<edit::Property> createEditableProperty() override;

private:
    NodeInfo* createNewMeshNode(const std::string& name);
    void initFromBinary(const Config& config);
    void processBinaryNode();
    void extractAnimations(const std::string& fileName, BinaryData& animBinData, bool remapNodes);

private:
    std::unique_ptr<ModelBuilder> mBuilder;
    std::vector<NodeInfo> mNodes;
    std::map<std::string, AnimationPtr> mAnimations;
    std::vector<SkeletonInfo> mSkeletons;
    std::vector<BinaryData::Node> mNodesMap;

    bool mTransparent{false};
    AABB mAABB;

    std::map<std::string, FileItem> mFiles;
    std::vector<std::string> mErrors;

    std::set<std::string> mReferencedTextures;
    std::string mDetails;
    std::string mMaterialDetails;
    std::shared_ptr<BinaryData> mBinaryData;

    ObjectPtr mThumbnailTex;
    float mDefaultScale{1.f};
};
