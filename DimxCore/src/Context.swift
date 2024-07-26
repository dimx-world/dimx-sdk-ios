//
//  Context.swift
//  DimxCore
//
//  Created by Sergii Romanov on 21/07/2023.
//  Copyright © 2023 Dimensions. All rights reserved.
//

import Foundation
import UIKit
import AVFoundation
import DimxNative

func topMostViewController() -> UIViewController? {
    var topController: UIViewController? = UIApplication.shared.keyWindow?.rootViewController
    while let presentedViewController = topController?.presentedViewController {
        topController = presentedViewController
    }
    return topController
}

public class Context
{
    static var sInstance: Context!

    private let mSettings = AppSettings()
    private var mAppConfig: AppConfig!
    private var mLocationManager: LocationManager!
    private var mWindow: UIWindow!

    private var mARViewCtrl: ARViewCtrl!
    private var mWebViewCtrl: WebViewCtrl?

    private var mPermissionsGranted = false
    
    static public func initialize(_ window: UIWindow, _ appConfig: AppConfig) {
        if sInstance != nil {
            fatalError("Context already initialized")
        }
        sInstance = Context()
        sInstance.initializeInternal(window, appConfig)
    }

    static public func inst() -> Context {
        return sInstance
    }
    
    init(){}
    
    func initializeInternal(_ window: UIWindow, _ appConfig: AppConfig) {
        mAppConfig = appConfig
        mLocationManager = LocationManager()
        mWindow = window
    }

    public func permissionsGranted() -> Bool {
        return mPermissionsGranted
    }
    
    public func validatePermissions(_ callback: @escaping (Bool) -> Void) {
        if mPermissionsGranted {
            callback(true)
            return
        }
        checkPermissions({[self] success in
            mPermissionsGranted = success
            callback(success)
        })
    }
    
    public func settings() -> AppSettings {
        return mSettings
    }

    func appConfig() -> AppConfig {
        return mAppConfig
    }

    func locationManager() -> LocationManager {
        return mLocationManager
    }

    func arViewCtrl() -> ARViewCtrl {
        return mARViewCtrl;
    }

    func webViewCtrl() -> WebViewCtrl? {
        return mWebViewCtrl;
    }

    func showAlert(_ message: String, _ callback: @escaping () -> Void) {
        guard let topController = topMostViewController() else {
            Logger.error("No top controller found")
            return
        }
        
        let alertController = UIAlertController(title: nil, message: message, preferredStyle: .alert)
        alertController.addAction(UIAlertAction(title: "Go To Settings", style: .default, handler: { /*[alertController]*/ action in
            switch action.style{
                case .default:
                    UIApplication.shared.open(URL(string:UIApplication.openSettingsURLString)!)
                    break
                case .cancel: break
                case .destructive: break
                @unknown default: break
            }
        }))
        alertController.addAction(UIAlertAction(title: "Continue without AR", style: .default, handler: { /*[alertController]*/ action in
            switch action.style{
                case .default:
                    callback()
                    break
                case .cancel: break
                case .destructive: break
                @unknown default: break
            }
        }))
        
        topController.present(alertController, animated: true, completion: nil)
    }

    func checkPermissions(_ callback: @escaping (Bool) -> Void) {
        mLocationManager.requestPermission({ [self] granted in
            if !granted {
                Logger.error("Location access denied")
                showAlert("Please grant location permission to enable location-based AR functionality.", { [] in callback(false)})
                return
            }

            if AVCaptureDevice.authorizationStatus(for: .video) ==  .authorized {
                callback(true)
            } else {
                AVCaptureDevice.requestAccess(for: .video, completionHandler: { [self] (granted: Bool) in
                    DispatchQueue.main.async { [self] in
                        if !granted {
                            Logger.error("Camera access denied")
                            showAlert("Please grant camera permission to enable AR functionality.", {[] in callback(false)})
                            return
                        }
                        callback(true)
                    }
                })
            }
        })
    }

    public func showAppScreen(_ args: String) {
        mAppConfig.showAppScreenAction()?(args)
    }

    public func showARScreen(_ url: String, _ settingsData: String, _ accountData: String) {
        validatePermissions { [self] success in
            if success {
                if (mARViewCtrl == nil) {
                    mARViewCtrl = ARViewCtrl()
                    mARViewCtrl.initialUrl = url
                    mARViewCtrl.initialSettingsData = settingsData
                    mARViewCtrl.initialAccountData = accountData
                    mARViewCtrl.modalPresentationStyle = .fullScreen //or .overFullScreen for transparency
                } else {
                    reloadARSession(url, settingsData, accountData)
                }

                mWindow.rootViewController!.dismiss(animated: false)
                mWindow.rootViewController!.present(mARViewCtrl, animated: false, completion: nil)
            }
        }
    }

    public func showWebScreen(_ webUrl: String) {
        if (mWebViewCtrl == nil) {
            mWebViewCtrl = WebViewCtrl()
            mWebViewCtrl!.modalPresentationStyle = .fullScreen
        }

        mWindow.rootViewController!.dismiss(animated: false)
        mWindow.rootViewController!.present(mWebViewCtrl!, animated: false, completion: {[self, webUrl] in
            mWebViewCtrl!.loadWebUrl(webUrl)
        })
    }

    public func convertAppUrlToWebUrl(_ appUrl: String) -> String {
        let stringObj = String_create(UnsafeRawPointer(bitPattern: 0))
        cppConvertAppUrlToWebUrl(mSettings.webAppHost(), appUrl, stringObj)
        let webUrl = String(cString: String_cstr(stringObj))
        String_delete(stringObj)
        return webUrl
    }
    
    public func reloadARSession(_ url: String, _ settingsData: String, _ accountData: String) {
        DeviceAR.instance.restartSession()
        reloadEngineSession(url, settingsData, accountData)
    }

    public func getInterfaceOrientation() -> UIInterfaceOrientation {
        if let windowScene = mARViewCtrl.view.window?.windowScene {
            return windowScene.interfaceOrientation
        }
        return .portrait
    }
}
