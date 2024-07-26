#pragma once
#include <Common.h>
#include <EventPublisher.h>
#include <world/DimensionInfo.h>

DECL_ENUM(AccountEvent,  ChangeAccount,   UpdateDimensions);
DECL_ESTR(AccountEvent, "ChangeAccount", "UpdateDimensions");

class UserAccount;
class AccountManager
{
    using LoginCallback = std::function<void(const std::string&)>;
    DECLARE_EVENTS(AccountEvent)

public:
    static void saveAccountData(const std::string& data);
    
public:
    AccountManager();
    ~AccountManager();
    void initialize(const Config& config);
    void deinitialize();

    void localReload();

    void login(const std::string& email, const std::string& pass, LoginCallback callback);
    void logout();

    void reloadAccount(std::function<void(const std::string&)> callback);
    void reloadDimensions(std::function<void(const std::string&)> callback);

    const UserAccount& user() const;
    bool isDefault() const;

    const std::vector<const DimensionInfo*>& dimensions() const;
    const DimensionInfo* getDimension(ObjectId id) const;

    void setCurrentAccount(const Config& config);

    void enableAccountDimension(ObjectId dim, bool enabled);

private:
    void loginWithHashedPass(const std::string& email, const std::string& pass, LoginCallback callback);
    void clearDimensions();
    void updateDimensions(const Config& config);

private:
    std::unique_ptr<UserAccount> mCurrentUser;
    std::map<ObjectId, DimensionInfo> mDimensionsMap;
    std::vector<const DimensionInfo*> mDimensionsList;
    ObjectPtr mDefaultDimensionIcon;
};
