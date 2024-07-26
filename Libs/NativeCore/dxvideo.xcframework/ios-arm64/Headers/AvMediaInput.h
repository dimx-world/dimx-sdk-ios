#pragma once
#include <multimedia/MediaInput.h>
#include "AvMediaInputWorker.h"

class AvInputContext;
class AvMediaInput : public MediaInput
{
public:
    AvMediaInput(const std::string& url, std::unique_ptr<AvInputContext> inputContext, bool forceMonoChannel, bool alphaMatte);
    ~AvMediaInput();
    void seekToBegin() override;
    void setPlaybackState(PlaybackState state) override;

private:
    AvMediaInputWorker mWorker;
};
