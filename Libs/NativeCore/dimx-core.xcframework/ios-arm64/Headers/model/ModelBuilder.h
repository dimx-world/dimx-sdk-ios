#pragma once
#include <config/Config.h>

namespace edit {
class Property;
}
class Model;
class ModelBuilder
{
public:
    ModelBuilder(Model& model, const Config& config);
    virtual ~ModelBuilder() = default;
    
    virtual void serialize(Config& out);

    Model& model();
    const std::string& type() const;

    const std::shared_ptr<edit::Property>& editableProperty();

protected:
    virtual std::shared_ptr<edit::Property> createEditableProperty() = 0;

private:
    Model& mModel;
    std::string mType;
    std::shared_ptr<edit::Property> mEditableProperty;
};
