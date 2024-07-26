#pragma once

#include "Common.h"
#include <ecs/Component.h>
#include <config/Config.h>
#include "Texture.h"
#include "BinaryData.h"
#include <model/VertexFormat.h>
#include <edit/PropertyGroup.h>
#include <Counter.h>

#include <optional>

class ResourceManager;

DECL_ENUM(MatParamType,  Bool,   Int,   UInt,   Float,   Vec2,   Vec3,   Vec4,   Color,   Texture,   String);
DECL_ESTR(MatParamType, "Bool", "Int", "UInt", "Float", "Vec2", "Vec3", "Vec4", "Color", "Texture", "String");

DECL_ENUM(StencilMode,  Read,   Write);
DECL_ESTR(StencilMode, "Read", "Write");

DECL_ENUM(StencilFunction,  Never,   Less,   LessOrEqual,   Greater,   GreaterOrEqual,   Equal,   NotEqual,   Always);
DECL_ESTR(StencilFunction, "Never", "Less", "LessOrEqual", "Greater", "GreaterOrEqual", "Equal", "NotEqual", "Always");

class NativeMaterial;

class Material: public Component
{
public:
    DECLARE_COMPONENT(Material)

public:
    struct ParamInfo {
        ParamInfo() = default;
        ParamInfo(const std::string& nm, const Config& config);
        std::string name;
        MatParamType type;
        MaterialValue defaultValue;
        std::string glUniform;
        std::string glFragmentSwitch;
        //std::string glUniform;
        bool editable{false};
    };

    struct StencilInfo {
        StencilMode mode{StencilMode::None};
        int refValue{0};
        StencilFunction function{StencilFunction::None};

        StencilInfo() = default;
        StencilInfo(const Config& config);
        void serialize(Config& out);
    };

public:
    static bool isMaterialFile(const std::string& file);
    static ConfigPtr makeConfig(Config baseConfig, const std::string& filePath = {});

public:
    Material(Object* entity, const Config& config);
    virtual ~Material();
    void initialize(CounterPtr counter) override;

    void serialize(Config& out) override;

    Material* baseMaterial();
    const std::string& modelMaterialName() const { return mModelMaterialName; }

    bool hasOverrides() const;
    const std::map<std::string, MaterialValue>& overrides() const { return mOverrides; }

    static void populateConfigFromBinary(const BinaryData::Material& matInfo,
                                         const Config& matOverride,
                                         const std::string& baseDir,
                                         Config& outConfig);

    NativeMaterial& native() const;
    void setNative(std::unique_ptr<NativeMaterial> native);

    void resetBaseMaterial(ObjectPtr mat);
    void resetTextureParameter(const std::string& param, ObjectPtr tex);

    std::shared_ptr<edit::Property> createEditableProperty() override;

    std::vector<const ParamInfo*> getAllParams() const;

    const MaterialValue& getRawValue(const std::string& name) const;

    const Texture* tryGetTexture(const std::string& name) const;

    template <typename T>
    const T& getValue(const std::string& name) const;

    template <typename T>
    void setValue(const std::string& name, const T& value);

    VertexFormat vertexFormat() const;
    bool morphTargets() const;
    bool morphTargetsNormals() const;
    bool morphTargetsTangents() const;

    const ObjectPtr& glVertexShader() const;
    const ObjectPtr& glFragmentShader() const;

    void setTransparent(bool value) { mTransparent = value; }
    bool transparent() const;

    bool madeFromFile() const { return mMadeFromFile; }

    const StencilInfo& getStencil() const;
    
    void setStencilMode(StencilMode mode);
    void setStencilRefValue(int value);
    void setStencilFunction(StencilFunction func);

protected:
    void propagateParamUpdate(const std::string& name, const MaterialValue& value, bool skipOverridesCheck);

private:
    void initWithBaseMaterial(ObjectPtr baseMat, CounterPtr counter);
    void rebuildNative(CounterPtr counter = {});
    const ParamInfo& getParamInfo(const std::string& name) const;

private:
    ObjectPtr mBaseMaterial;
    bool mMadeFromFile{false};

    std::optional<bool> mTransparent;
    
    std::map<std::string, ParamInfo> mParams;
    std::map<std::string, MaterialValue> mOverrides;

    mutable std::unique_ptr<NativeMaterial> mNative;

    VertexFormat mVertexFormat;
    bool mMorphTargets{false};
    bool mMorphTargetsNormals{false};
    bool mMorphTargetsTangents{false};

    std::string mGlVertexShaderPath;
    ObjectPtr mGlVertexShader;
    std::string mGlFragmentShaderPath;
    ObjectPtr mGlFragmentShader;

    std::string mModelMaterialName;
    ObjectId mModelMaterialId;

    std::optional<StencilInfo> mStencil;
};

////////////////////////////////////////////////////////////////////////////////

template <> const Texture& Material::getValue(const std::string& name) const;

template <typename T>
const T& Material::getValue(const std::string& name) const
{
    try {
        return std::get<T>(getRawValue(name));
    } catch (const std::exception& e) {
        ASSERT(false, "Invalid type requested for material parameter [" << name << "]. Error: " << e.what());
    }
    static const MaterialValue _empty;
    return std::get<T>(_empty);
}

template <typename T>
void Material::setValue(const std::string& name, const T& value)
{
    MaterialValue& overValue = mOverrides[name];
    overValue = value;
    propagateParamUpdate(name, overValue, true);
}
