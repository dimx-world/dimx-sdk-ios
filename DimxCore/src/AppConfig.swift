//
//  AppConfig.swift
//  DimxCore
//
//  Created by Sergii Romanov on 21/07/2023.
//  Copyright © 2023 Dimensions. All rights reserved.
//

import Foundation
import UIKit

public class AppConfig
{
    private var mDimensions: [String] = []
    private var mShowAppScreenAction: ((String) -> Void)?
    private var mQRCodeEnabled: Bool = false
    private var mSharePhotoEnabled: Bool = false
    private var mShareVideoEnabled: Bool = false
    private var mWebVersions: [String] = []

    public init() {}

    public func addDimension(_ id: String) {
        mDimensions.append(id)
    }

    public func setShowAppScreenAction(_ callback: ((String) -> Void)?) {
        mShowAppScreenAction = callback
    }

    func showAppScreenAction() -> ((String) -> Void)? {
        return mShowAppScreenAction
    }

    public func setQRCodeEnabled(_ value: Bool) {
        mQRCodeEnabled = value
    }

    public func setSharePhotoEnabled(_ value: Bool) {
        mSharePhotoEnabled = value
    }
    
    public func setShareVideoEnabled(_ value: Bool) {
        mShareVideoEnabled = value
    }
    
    public func addWebVersion(_ url: String) {
        mWebVersions.append(url)
    }
    
    public func webVersions() -> [String] {
        return mWebVersions
    }
    
    func toJsonString() -> String {
        var jsonObject: [String: Any] = [
            "back_enabled": mShowAppScreenAction != nil,
            "qrcode_enabled": mQRCodeEnabled,
            "share_photo_enabled": mSharePhotoEnabled,
            "share_video_enabled": mShareVideoEnabled
        ]
        if mDimensions.count > 0 {
            jsonObject["dimensions"] = mDimensions
        }
        do {
            let jsonData = try JSONSerialization.data(withJSONObject: jsonObject)
            let str = String(data: jsonData, encoding: .utf8)
            return str != nil ? str! : ""
        } catch {
            Logger.error("Error creating AppConfig JSON data: \(error)")
        }
        return ""
    }
}
