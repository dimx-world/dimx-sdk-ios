#pragma once
#include <Common.h>
#include <config/Config.h>

struct DimentionRecord
{
    bool own{false};
    bool admin{false};
    bool edit{false};
    bool view{false};
    bool enabled{false};
    bool favorite{false};
    std::string env{};
    DimentionRecord(const Config& config)
    : own(config.get("own", false))
    , admin(config.get("admin", false))
    , edit(config.get("edit", false))
    , view(config.get("view", false))
    , enabled(config.get("enabled", false))
    , favorite(config.get("favorite", false))
    , env(config.get("env", ""))
    {
        if (env.empty()) {
            env = "prod";
        }
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
        return iter != mDimensions.end() && (iter->second.own || iter->second.admin || iter->second.edit);
    }

private:
    std::string mId;
    std::string mEmail;
    bool mAdmin{false};
    bool mEditor{false};
    std::map<ObjectId, DimentionRecord> mDimensions;
    bool mDeveloper{false};
};
