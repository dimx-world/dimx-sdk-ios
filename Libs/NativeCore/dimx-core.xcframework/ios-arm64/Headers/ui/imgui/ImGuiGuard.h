#pragma once
#include <Common.h>
#include <imgui.h>
#include <imgui_internal.h>

class ImGuiGuard
{
    NO_COPY_MOVE(ImGuiGuard)
public:
    ImGuiGuard() = default;
    ~ImGuiGuard() {
        if (mIdCounter > 0) {
            ImGui::PopID();
        }
        if (mItemFlagCounter > 0) {
            ImGui::PopItemFlag();
        }
        ImGui::PopStyleVar(mVarsCounter);
        ImGui::PopStyleColor(mColorCounter);
        if (mTextWrapPosCounter > 0) {
            ImGui::PopTextWrapPos();
        }
        tryPopFont();
    }

    void pushID(void* id) {
        ASSERT(mIdCounter == 0, "ID already pushed");
        ImGui::PushID(id);
        mIdCounter++;
    }

    void pushItemFlag(ImGuiItemFlags option, bool enabled) {
        ASSERT(mItemFlagCounter == 0, "Item flag already pushed");
        ImGui::PushItemFlag(option, enabled);
        mItemFlagCounter++;
    }

    void pushStyleColor(ImGuiCol idx, ImU32 col) {
        ImGui::PushStyleColor(idx, col);
        mColorCounter++;
    }

    void pushStyleColor(ImGuiCol idx, const ImVec4& col) {
        ImGui::PushStyleColor(idx, col);
        mColorCounter++;
    }

    void pushStyleColor(ImGuiCol idx, const Vec4& col) {
        ImGui::PushStyleColor(idx, ImVec4{col.x, col.y, col.z, col.w});
        mColorCounter++;
    }

    void pushStyleVar(ImGuiStyleVar idx, float val) {
        ImGui::PushStyleVar(idx, val);
        mVarsCounter++;
    }

    void pushStyleVar(ImGuiStyleVar idx, const ImVec2& val) {
        ImGui::PushStyleVar(idx, val);
        mVarsCounter++;
    }

    void pushStyleVar(ImGuiStyleVar idx, const Vec2& val) {
        ImGui::PushStyleVar(idx, ImVec2{val.x, val.y});
        mVarsCounter++;
    }

    void pushTextWrapPos(float pos) {
        ImGui::PushTextWrapPos(pos);
        mTextWrapPosCounter++;
    }

    void pushFont(const Font* font) {
        mPrevFontScale = ImGui::GetCurrentWindow()->FontWindowScale;
        ImGui::SetWindowFontScale(font->scale());
        ImGui::PushFont(font->imguiFont());
    }

    void tryPopFont() {
        if (mPrevFontScale > 0.f) {
            ImGui::SetWindowFontScale(mPrevFontScale);
            mPrevFontScale = -1.f;
            ImGui::PopFont();
        }
    }

private:
    int mIdCounter{0};
    int mItemFlagCounter{0};
    int mVarsCounter{0};
    int mColorCounter{0};
    int mTextWrapPosCounter{0};
    float mPrevFontScale{-1.f};
};