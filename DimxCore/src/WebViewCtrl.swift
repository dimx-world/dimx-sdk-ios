//
//  WebViewController.swift
//  dimx-ios-app
//
//  Created by Sergii Romanov on 24/07/2022.
//  Copyright © 2022 Dimensions. All rights reserved.
//

import UIKit
import WebKit
import CoreLocation
import DimxNative

class WebViewCtrl: UIViewController, WKUIDelegate, WKScriptMessageHandler, WKNavigationDelegate {
    //static private var startupAppUrl: String = ""

    var webView: WKWebView!
    var versionChecked = false
    var versionReloaded = false
    
    
    func loadWebUrl(_ webUrl: String) {
        Logger.info("loadAppUrl: \(webUrl)")

        if (!webUrl.isEmpty) {
            webView.load(URLRequest(url: URL(string: webUrl)!))
            if !versionChecked {
                versionChecked = true
                checkWebVersions()
            }
        } else {
            let jscode =
                """
                if (window.DimxInterface && window.DimxInterface.reloadAccount) {
                    window.DimxInterface.reloadAccount();
                }
                """
            webView.evaluateJavaScript(jscode) {
                (_, error) in
                if error != nil {
                    Logger.error("JS CALL ERROR: \(String(describing: error))")
                }
            }
        }
        
    }
    
    override func loadView() {
        let config = WKWebViewConfiguration()
        config.userContentController.add(self, name: "WebViewCtrl")

        //--- Inject AppInstanceId
        let appInstContent = "window.DIMX_APP_INSTANCE_ID = '\(Context.inst().settings().appInstanceId())'"
        let appInstScript = WKUserScript(source: appInstContent, injectionTime: .atDocumentStart, forMainFrameOnly: true)
        config.userContentController.addUserScript(appInstScript)

        //--- Inject WebInterface
        let filepath = Bundle.module.path(forResource: "WebInterface", ofType: "js")!
        var scriptContent = ""
        do {
           scriptContent = try String(contentsOfFile: filepath)
        } catch {
            fatalError("Failed to load web user script from file!")
        }
        let script = WKUserScript(source: scriptContent, injectionTime: .atDocumentStart, forMainFrameOnly: true)
        config.userContentController.addUserScript(script)
        
        //--- enable audio/video autoplay
        config.mediaTypesRequiringUserActionForPlayback = []
        //---
        
        webView = WKWebView(frame: .zero, configuration: config)
        webView.uiDelegate = self
        webView.navigationDelegate = self
        if #available(iOS 16.4, *) {
            webView.isInspectable = true
        } else {
            // Fallback on earlier versions
        }
/*
        webView.scrollView.bounces = false
        webView.scrollView.alwaysBounceVertical = false
        webView.scrollView.alwaysBounceHorizontal = false
*/
        self.view = webView
        
        NotificationCenter.default.addObserver(self, selector: #selector(appDidEnterBackground), name: UIApplication.didEnterBackgroundNotification, object: nil)
        NotificationCenter.default.addObserver(self, selector: #selector(appWillEnterForeground), name: UIApplication.willEnterForegroundNotification, object: nil)
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        //loadAppUrl(WebViewCtrl.startupAppUrl)
/*
        webView.scrollView.bounces = false
        webView.scrollView.alwaysBounceVertical = false
        webView.scrollView.alwaysBounceHorizontal = false
        if #available(iOS 17.4, *) {
            webView.scrollView.bouncesVertically = false
            webView.scrollView.bouncesHorizontally = false
        }
        webView.scrollView.bouncesZoom = false
*/
    }

    func userContentController(_ userContentController: WKUserContentController, didReceive message: WKScriptMessage) {
        let params = message.body as! [String: AnyObject]
        let cmd = params["command"] as! String
        if (cmd == "SHOW_AR") {
            Context.inst().showARScreen(params["url"] as! String, params["settings"] as! String, params["account"] as! String)
            return
        } else if (cmd == "SET_WEB_APP_HOST") {
            Context.inst().settings().setWebAppHost(params["value"] as! String)
            return
        } else if (cmd == "REQUEST_TRACKING_STATUS") {
            let stringObj = String_create(UnsafeRawPointer(bitPattern: 0))
            getAnchorsTrackingStatus(params["dimension"] as! String, stringObj)
            let info = String(cString: String_cstr(stringObj))
            String_delete(stringObj)
            let jscode =
                """
                if (window.DimxInterface) {
                    if (window.DimxInterface.updateTrackingStatus) {
                        window.DimxInterface.updateTrackingStatus(JSON.parse('\(info)'))
                    } else {
                        console.error('FROM SWIFT: window.DimxInterface.updateTrackingStatus not defined')
                    }
                } else {
                    console.error('FROM SWIFT: window.DimxInterface not defined')
                }
                """;
            webView.evaluateJavaScript(jscode) {
                (_, error) in
                if error != nil {
                    Logger.error("JS CALL ERROR: \(String(describing: error))")
                }
            }
            return
        } else if (cmd == "REQUEST_GEOLOCATION_UPDATE") {
            if let loc = Context.inst().locationManager().location() {
                onsGeolocationUpdate(loc)
            }
            return
        }
       
        fatalError("Unknown web command: [" + cmd + "]")
    }

    func onsGeolocationUpdate(_ loc: CLLocation) {
        let geoStr = "\(loc.coordinate.latitude) \(loc.coordinate.longitude) \(loc.altitude) \(loc.horizontalAccuracy) \(loc.verticalAccuracy)"
        let jscode =
            """
            if (window.DimxInterface) {
                if (window.DimxInterface.updateGeolocation) {
                    window.DimxInterface.updateGeolocation('\(geoStr)')
                } else {
                    console.error('FROM JAVA: window.DimxInterface.updateGeolocation not defined')
                }
            } else {
                console.error('FROM JAVA: window.DimxInterface not defined')
            }
            """;
        webView.evaluateJavaScript(jscode) {
            (_, error) in
            if error != nil {
                Logger.error("JS CALL ERROR: \(String(describing: error))")
            }
        }
    }
    
    func webView(_ webView: WKWebView,
                 runJavaScriptAlertPanelWithMessage message: String,
                 initiatedByFrame frame: WKFrameInfo,
                 completionHandler: @escaping () -> Void)
    {
        let alertController = UIAlertController(title: nil, message: message, preferredStyle: .alert)
        alertController.addAction(UIAlertAction(title: "Ok", style: .default, handler: { (action) in
            completionHandler()
        }))
        self.present(alertController, animated: true, completion: nil)
    }

    func webView(_ webView: WKWebView,
                 runJavaScriptConfirmPanelWithMessage message: String,
                 initiatedByFrame frame: WKFrameInfo,
                 completionHandler: @escaping (Bool) -> Void)
    {
/*
 Note: for preferredStyle: .actionSheet
        alertController.popoverPresentationController?.sourceView = self.view
        alertController.popoverPresentationController?.sourceRect = CGRect(x: 0, y: 0, width: 0, height: 0)
        alertController.addAction(UIAlertAction(title: "Cancel", style: .cancel, handler: { (action) in
            completionHandler(false)
        }))
 */
        let alertController = UIAlertController(title: nil, message: message, preferredStyle: .alert)
        
        alertController.addAction(UIAlertAction(title: "Ok", style: .default, handler: { (action) in
            completionHandler(true)
        }))
        alertController.addAction(UIAlertAction(title: "Cancel", style: .default, handler: { (action) in
            completionHandler(false)
        }))

        self.present(alertController, animated: true, completion: nil)
    }

    func webView(_ webView: WKWebView,
                 runJavaScriptTextInputPanelWithPrompt prompt: String,
                 defaultText: String?,
                 initiatedByFrame frame: WKFrameInfo,
                 completionHandler: @escaping (String?) -> Void)
    {
        let alertController = UIAlertController(title: nil, message: prompt, preferredStyle: .alert)
        alertController.addTextField { (textField) in
            textField.text = defaultText
        }
        alertController.addAction(UIAlertAction(title: "Ok", style: .default, handler: { (action) in
            if let text = alertController.textFields?.first?.text {
                completionHandler(text)
            } else {
                completionHandler(defaultText)
            }
        }))
        alertController.addAction(UIAlertAction(title: "Cancel", style: .default, handler: { (action) in
            completionHandler(nil)
        }))
        self.present(alertController, animated: true, completion: nil)
    }

    // WKNavigationDelegate method
    func webView(_ webView: WKWebView, decidePolicyFor navigationAction: WKNavigationAction, decisionHandler: @escaping (WKNavigationActionPolicy) -> Void) {
        
        //Logger.info("navigationAction 0: \(String(describing: navigationAction.request.url?.absoluteString))")
        
        if navigationAction.request.url?.absoluteString == "about:blank" {
            Logger.info("Allowing about:blank redirect")
            decisionHandler(.allow)
            return
        }

        if navigationAction.request.url?.host == "dimx-world.firebaseapp.com" {
            Logger.info("Allowing dimx-world.firebaseapp.com redirect")
            decisionHandler(.allow)
            return
        }
        
        if navigationAction.request.url == nil {
            decisionHandler(.allow)
            return
        }

        let host = navigationAction.request.url!.host!
        if host.contains("dimx.world") || host == "localhost" || host == "sergei-laptop" {
            decisionHandler(.allow)
            return
        }

        UIApplication.shared.open(navigationAction.request.url!, options: [:], completionHandler: nil)
        decisionHandler(.cancel)
    }
    
    func checkWebVersions() {
        for versionUrl in Context.inst().appConfig().webVersions() {
            Logger.info("Checking web version: \(versionUrl)")
            
            guard let url = URL(string: versionUrl) else {
                Logger.info("Invalid version url: \(versionUrl)")
                return
            }
            
            let request = URLRequest(url: url, cachePolicy: .reloadIgnoringLocalAndRemoteCacheData)
            let task = URLSession.shared.dataTask(with: request) { [weak self] data, response, error in
                guard let strongSelf = self else {
                    return
                }

                guard let httpResponse = response as? HTTPURLResponse else {
                    return
                }
                
                if httpResponse.statusCode < 200 || httpResponse.statusCode > 299 {
                    Logger.info("Failed to fetch web version [\(versionUrl)]. Status code [\(httpResponse.statusCode)]")
                    return
                }
                
                if let data = data, let version = String(data: data, encoding: .utf8) {
                    let cachedVersion = Context.inst().settings().getWebVersion(versionUrl)
                    Logger.info("Web versions [\(versionUrl)] cached [\(String(describing: cachedVersion))] latest [\(version)]")

                    if version == cachedVersion {
                        return
                    }
                    Context.inst().settings().setWebVersion(versionUrl, version)

                    if cachedVersion == nil {
                        return
                    }

                    Logger.info("Version changed [\(versionUrl)]. Requesting reload.")
                    DispatchQueue.main.async {
                        if !strongSelf.versionReloaded {
                            strongSelf.versionReloaded = true
                            
                            let websiteDataTypes = NSSet(array: [WKWebsiteDataTypeDiskCache, WKWebsiteDataTypeMemoryCache])
                            let date = Date(timeIntervalSince1970: 0)
                            Logger.info("Cleaning web cache..")
                            WKWebsiteDataStore.default().removeData(ofTypes: websiteDataTypes as! Set<String>, modifiedSince: date, completionHandler: {
                                if strongSelf.webView.url != nil {
                                    Logger.info("Reloading url: \(String(describing: strongSelf.webView.url))")
                                    strongSelf.webView.load(URLRequest(url: strongSelf.webView.url!))
                                }
                            })
                        }
                    }
                }
            }
            
            task.resume()
        }
    }
    
    func notifyWebViewHide() {
        let jscode =
            """
            if (window.DimxInterface) {
                window.DimxInterface.onWebViewHide()
            } else {
                console.error('FROM JAVA: window.DimxInterface not defined')
            }
            """;
        webView.evaluateJavaScript(jscode) {
            (_, error) in
            if error != nil {
                Logger.error("JS CALL ERROR: \(String(describing: error))")
            }
        }
    }

    func notifyWebViewShow() {
        let jscode =
            """
            if (window.DimxInterface) {
                window.DimxInterface.onWebViewShow()
            } else {
                console.error('FROM JAVA: window.DimxInterface not defined')
            }
            """;
        webView.evaluateJavaScript(jscode) {
            (_, error) in
            if error != nil {
                Logger.error("JS CALL ERROR: \(String(describing: error))")
            }
        }
    }
    
    override func viewWillDisappear(_ animated: Bool) {
        super.viewWillDisappear(animated)
        print("WebView will disappear")
        notifyWebViewHide()
        UIApplication.shared.isIdleTimerDisabled = false
    }
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillDisappear(animated)
        print("WebView will appear")
        notifyWebViewShow()
        UIApplication.shared.isIdleTimerDisabled = true
    }
/*
    override func viewDidLayoutSubviews() {
        super.viewDidLayoutSubviews()
        webView.frame = view.bounds
    }
*/
    @objc func appDidEnterBackground() {
        if view.window != nil {
            print("WebViewCtrl: app did enter background")
            notifyWebViewHide()
        }
    }
    
    @objc func appWillEnterForeground() {
        if view.window != nil {
            print("WebViewCtrl: app will enter foreground")
            notifyWebViewShow()
        }
    }
}
