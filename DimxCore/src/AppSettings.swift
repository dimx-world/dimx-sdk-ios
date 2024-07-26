//
//  AppSettings.swift
//  dimx-ios-app
//
//  Created by Sergii Romanov on 10/12/2022.
//  Copyright © 2022 Dimensions. All rights reserved.
//

import Foundation

public class AppSettings {
    static private let APP_INSTANCE_ID_KEY = "app_instance_id"
    static private let WEB_APP_HOST_KEY = "web_app_host"
    static private let WEB_VERSION_KEY = "web_version_"
    
    private var mAppInstanceId: String!
    private var mWebAppHost: String!
   
    func appInstanceId() -> String {
        if mAppInstanceId != nil {
            return mAppInstanceId
        }
        mAppInstanceId = UserDefaults.standard.string(forKey: AppSettings.APP_INSTANCE_ID_KEY)
        if mAppInstanceId == nil {
            mAppInstanceId = UUID().uuidString
            UserDefaults.standard.setValue(mAppInstanceId, forKey: AppSettings.APP_INSTANCE_ID_KEY)
            Logger.info("Generated new app_instance_id: \(String(describing: mAppInstanceId))")
        } else {
            Logger.info("Loaded existing app_instance_id: \(String(describing: mAppInstanceId))")
        }
        return mAppInstanceId
    }

    public func webAppHost() -> String {
        if mWebAppHost != nil {
            return mWebAppHost
        }
        mWebAppHost = UserDefaults.standard.string(forKey: AppSettings.WEB_APP_HOST_KEY)
        if mWebAppHost == nil {
            setWebAppHost("https://meta.dimx.world")
        } else {
            Logger.info("Loaded existing web app host: \(String(describing: mWebAppHost))")
        }
        return mWebAppHost
    }

    func setWebAppHost(_ value: String) {
        mWebAppHost = value
        UserDefaults.standard.setValue(mWebAppHost, forKey: AppSettings.WEB_APP_HOST_KEY)
        Logger.info("Setting web app host: \(String(describing: mWebAppHost))")
    }
    
    func getWebVersion(_ key: String) -> String? {
        return UserDefaults.standard.string(forKey: AppSettings.WEB_VERSION_KEY + key)
    }
    
    func setWebVersion(_ key: String, _ value: String) {
        UserDefaults.standard.setValue(value, forKey: AppSettings.WEB_VERSION_KEY + key)
    }
}
