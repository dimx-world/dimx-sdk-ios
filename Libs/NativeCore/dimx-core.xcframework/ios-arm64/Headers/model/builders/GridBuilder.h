#pragma once
#include <model/ModelBuilder.h>

class GridBuilder: public ModelBuilder
{
public:
    GridBuilder(Model& model, const Config& config);

    void serialize(Config& out) override;

    void setCellSize(float size, bool rebuild = true);
    void setNumCells(size_t num, bool rebuild = true);

protected:
    std::shared_ptr<edit::Property> createEditableProperty() override;

private:
    void rebuildNative();

private:
    float mCellSize{0.f};
    size_t mNumCells{0};
};
