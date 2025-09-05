#pragma once
#include <Common.h>
#include <config/Config.h>

DECL_ENUM(UserRole,  Admin,   Editor,   Viewer)
DECL_ESTR(UserRole, "Admin", "Editor", "Viewer")

struct DimentionRecord
{
    UserRole role{UserRole::None};
    bool enabled{true};
    bool favorite{false};
    std::string env{};

    DimentionRecord(const Config& config)
    : role(config.get("role", UserRole::None))
    , enabled(config.get("enabled", true))
    , favorite(config.get("favorite", false))
    , env(config.get("env", ""))
    {
        if (env.empty()) {
            env = "prod";
        }
        if (role == UserRole::None) {
            if (config.get("own", false) || config.get("admin", false)) {
                role = UserRole::Admin;
            } else if (config.get("edit", false)) {
                role = UserRole::Editor;
            }
        }
    }

    bool editable() const {
        return role == UserRole::Admin || role == UserRole::Editor;
    }
};

class UserAccount
{
    AccessLevel access{AccessLevel::None};

public:
    UserAccount(const Config& config);

    const std::string& id() const { return mId; }
    const std::string& email() const { return mEmail; }

    void setDimensions(const Config& config);
    const std::map<ObjectId, DimentionRecord>& dimensions() const { return mDimensions; }

    bool developer() const { return mDeveloper; }

    const std::string& dimensionEnv(ObjectId dimId) const {
        static const std::string _prod{"prod"};
        auto iter = mDimensions.find(dimId);
        return iter != mDimensions.end() ? iter->second.env : _prod;
    }

    bool canEdit(ObjectId dimId) const {
        auto iter = mDimensions.find(dimId);
        return iter != mDimensions.end() && iter->second.editable();
    }

private:
    std::string mId;
    std::string mEmail;
    bool mAdmin{false};
    bool mEditor{false};
    std::map<ObjectId, DimentionRecord> mDimensions;
    bool mDeveloper{false};
};
