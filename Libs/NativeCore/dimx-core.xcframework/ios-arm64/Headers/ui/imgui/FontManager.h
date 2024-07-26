#pragma once
#include <Common.h>
#include "Font.h"
#include <Buffer.h>
#include <config/Config.h>

struct ImFontAtlas;

class FontManager
{
    NO_COPY_MOVE(FontManager);

public:
    FontManager() = default;

    void initialize(const Config& config, CounterPtr counter);
    void deinitialize();

    ImFontAtlas* atlas() { return mFontAtlas.get(); }
    const ObjectPtr& texture() { return mTexture; }

    const Font* getFont(const std::string& name = {}, int size = -1);

private:
    void initFontsFromFiles(CounterPtr counter);
    ImFont* selectImFontBySize(const std::vector<ImFont*>& list, int fontSize);

private:
    std::unique_ptr<ImFontAtlas> mFontAtlas;
    ObjectPtr mTexture;
    std::map<std::string, std::vector<ImFont*>> mImguiFonts;
    std::map<std::string, std::map<int, Font>> mCache;

    std::map<std::string, BufferPtr> mFiles;

    std::string mDefaultFontName;
    int mDefaultFontSize{-1};
};