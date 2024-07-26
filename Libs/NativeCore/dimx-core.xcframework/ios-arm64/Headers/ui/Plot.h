#pragma once
#include "Element.h"

DECL_ENUM(SubPlotType,  Line,   Bars)
DECL_ESTR(SubPlotType, "Line", "Bars")

namespace ui {

class Plot;

class SubPlot
{
public:
    SubPlot(Plot* parent, SubPlotType type, const Config& config);
    virtual ~SubPlot() = default;

    virtual void update() = 0;

    SubPlotType type() const { return mType; }

    void setLabel(const std::string& label) { mLabel = label; }
    const std::string& label() const { return mLabel; }

    void setDataX(std::vector<double> data) { mDataX = std::move(data); }
    const std::vector<double>& dataX() const { return mDataX; };

    void setDataY(std::vector<double> data) { mDataY = std::move(data); }
    const std::vector<double>& dataY() const { return mDataY; };

private:
    Plot* mParent{nullptr};
    SubPlotType mType{SubPlotType::None};
    std::string mLabel;
    std::vector<double> mDataX;
    std::vector<double> mDataY;
};

class SubPlotLine: public SubPlot
{
public:
    SubPlotLine(Plot* parent, const Config& config);
    void update() override;
};

class SubPlotBars: public SubPlot
{
public:
    SubPlotBars(Plot* parent, const Config& config);
    void update() override;

private:
    double mBarSize{0.7};
};

DECLARE_PTR(Plot)
class Plot: public Element
{

public:
    Plot(UIType type, UIContext* context, Element* parent, const Config& config);
    Plot(UIContext* context, Element* parent, const Config& config);
    virtual ~Plot() = default;
    void update(const FrameContext& frameContext) override;

    const std::vector<std::unique_ptr<SubPlot>>& subPlots() { return mSubPlots; }

private:
    std::vector<std::unique_ptr<SubPlot>> mSubPlots;
};

} // namespace ui