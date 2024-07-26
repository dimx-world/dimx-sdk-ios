import MetalKit
import AVFoundation
import DimxNative

class CustomPanGestureRecognizer: UIPanGestureRecognizer {
  private var initialTouchLocation: CGPoint!

  override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent) {
    super.touchesBegan(touches, with: event)
    initialTouchLocation = touches.first!.location(in: view)
  }
}

class ARViewCtrl: UIViewController, MTKViewDelegate, UITextInputTraits {
    var initialUrl = ""
    var initialSettingsData = ""
    var initialAccountData = ""
    var isCurrentlyVisible = false
    var mExtMediaStore = ExtMediaStore()

    override func viewDidLoad() {
        super.viewDidLoad()

        self.view = ARView()

        do {
            let session = AVAudioSession.sharedInstance()
            try session.setCategory(.playback)
            try session.setActive(true)
        } catch {
            Logger.error("Error setting audio session category or activating audio session: \(error)")
        }

        initCallbacks();

        let mtkView = self.view as! MTKView
        mtkView.device = MTLCreateSystemDefaultDevice()
        mtkView.delegate = self
        mtkView.framebufferOnly = false
        mtkView.colorPixelFormat = .rgba8Unorm // Set pixel format to RGBA. Default 80 = bgra8Unorm
        Logger.info("Color pixel format: \(mtkView.colorPixelFormat.rawValue)")
        //mtkView.depthStencilPixelFormat = .depth32Float
        mtkView.depthStencilPixelFormat = .depth32Float_stencil8
        mtkView.clearDepth = 1.0

        Renderer.initSingleton(mtkView.device!, mtkView)

        initAnalyticsInfo()

        let libPath = try! FileManager.default.url(for: .libraryDirectory, in: .userDomainMask, appropriateFor: nil, create: false).path

        Logger.info("SWIFT: initializing engine: \(initialUrl)")
        initEngine(Context.inst().settings().appInstanceId(),
                   Bundle.module.bundlePath.appending("/data"),
                   libPath + "/LocalStorage",
                   libPath + "/Caches",
                   "ExtMedia",
                   initialUrl,
                   initialSettingsData,
                   initialAccountData,
                   Context.inst().appConfig().toJsonString())
        
        //Adding notifies on keyboard appearing
        NotificationCenter.default.addObserver(self, selector: #selector(keyboardWasShown(_:)), name: UIResponder.keyboardWillShowNotification, object: nil)
        NotificationCenter.default.addObserver(self, selector: #selector(keyboardWillBeHidden(_:)), name: UIResponder.keyboardWillHideNotification, object: nil)
        NotificationCenter.default.addObserver(self, selector: #selector(appWillEnterForeground), name: UIApplication.willEnterForegroundNotification, object: nil)
        
        UIDevice.current.beginGeneratingDeviceOrientationNotifications()
        NotificationCenter.default.addObserver(self, selector: #selector(orientationDidChange), name: UIDevice.orientationDidChangeNotification, object: nil)
    }

    func initCallbacks() {
        // Engine callbacks
        g_swiftEngine().pointee.screenWidth = { [] () -> Int in return Int(Renderer.instance.viewportSize.width) }
        g_swiftEngine().pointee.screenHeight = { () -> Int in return Int(Renderer.instance.viewportSize.height) }
        g_swiftEngine().pointee.showKeyboard = { () -> Void in Context.inst().arViewCtrl().showKeyboard() }
        g_swiftEngine().pointee.hideKeyboard = { () -> Void in Context.inst().arViewCtrl().hideKeyboard() }
        g_swiftEngine().pointee.showAppScreen = { (url: UnsafePointer<CChar>!) -> Void in Context.inst().showAppScreen(String(cString: url)) }
        g_swiftEngine().pointee.openUrlExternal = { (url: UnsafePointer<CChar>!) -> Void in Context.inst().arViewCtrl().openUrlExternal(String(cString: url)) }
        g_swiftEngine().pointee.requestGeolocationUpdate = { () -> Void in Context.inst().arViewCtrl().requestGeolocationUpdate() }
        g_swiftEngine().pointee.moveToExtMediaFile = { (args: UnsafePointer<CChar>!) -> Void in Context.inst().arViewCtrl().moveToExtMediaFile(String(cString: args)) }
        g_swiftEngine().pointee.shareExtMediaFile = { (args: UnsafePointer<CChar>!) -> Void in Context.inst().arViewCtrl().shareExtMediaFile(String(cString: args)) }
        
        Renderer.initCallbacks()
        DeviceAR.initCallbacks()
        AnchorSession.initCallbacks()
        Texture.initCallbacks()
        Material.initCallbacks()
        Mesh.initCallbacks()
        Renderable.initCallbacks()
    }

    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
//        let configuration = ARWorldTrackingConfiguration()
//        configuration.planeDetection = .horizontal
//        session.run(configuration)
    }
    
    override func viewWillDisappear(_ animated: Bool) {
        super.viewWillDisappear(animated)
        DeviceAR.instance.pauseSession()
    }
    
    override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)
        isCurrentlyVisible = true
        UIApplication.shared.isIdleTimerDisabled = true
    }

    override func viewDidDisappear(_ animated: Bool) {
        super.viewDidDisappear(animated)
        isCurrentlyVisible = false
        UIApplication.shared.isIdleTimerDisabled = false
    }
    
    func mtkView(_ view: MTKView, drawableSizeWillChange size: CGSize) {
//        renderer.drawRectResized(size: size)
    }
    
    func draw(in view: MTKView) {
        updateEngine()
    }
/*
    static func showAR(_ url: String, _ settingsData: String, _ accountData: String) {
        if (ARViewCtrl.instance == nil) {
            let storyBoard = UIStoryboard(name: "Main", bundle: nil)
            ARViewCtrl.instance = (storyBoard.instantiateViewController(withIdentifier: "ARView") as! ARViewCtrl)
            ARViewCtrl.instance!.processUrl = url
            ARViewCtrl.instance!.settingsData = settingsData
            ARViewCtrl.instance!.accountData = accountData
        } else {
            reloadEngineSession(url, settingsData, accountData)
        }
       
        WebViewCtrl.instance!.navigationController!.show(ARViewCtrl.instance!, sender: nil)
    }

    static func hideAR() {
        ARViewCtrl.instance!.navigationController!.popViewController(animated: true)
    }
*/
    func openUrlExternal(_ urlString: String) {
        if let url = URL(string: urlString) {
            if UIApplication.shared.canOpenURL(url) {
                UIApplication.shared.open(url, options: [:], completionHandler: nil)
            } else {
                Logger.error("Unable to open external  URL: \(urlString)")
            }
        } else {
            Logger.error("Invalid external URL: \(urlString)")
        }
    }

    func initAnalyticsInfo() {
        AnalyticsManager_setOSVersion(UIDevice.current.systemVersion)
        
        var systemInfo = utsname()
        uname(&systemInfo)
        let deviceModel = withUnsafePointer(to: &systemInfo.machine) {
            $0.withMemoryRebound(to: CChar.self, capacity: 1) {
                ptr in String.init(validatingUTF8: ptr)
            }
        }
        AnalyticsManager_setDeviceModel(deviceModel)
    }

    func showKeyboard() {
        if (!view.isFirstResponder) {
            view.becomeFirstResponder()
        }
    }
    func hideKeyboard() {
        if (view.isFirstResponder) {
            view.resignFirstResponder()
        }
    }
    
    @objc func keyboardWasShown(_ notification: Notification) {
        let userInfo = notification.userInfo!
        let keyboardSize = (userInfo[UIResponder.keyboardFrameEndUserInfoKey] as AnyObject).cgRectValue.size
        let keyboardTop = view.frame.size.height - keyboardSize.height
        
        let viewport = Renderer.instance.viewportSize
        let bounds = view.bounds
        let pixelsTop = Float(keyboardTop / bounds.height * viewport.height)
        
        setKeyboardTop(pixelsTop);
    }
    
    @objc func keyboardWillBeHidden(_ notification: Notification) {
        setKeyboardTop(0.0); // 0.0 is a special value
    }
    
    @objc func appWillEnterForeground() {
        if isCurrentlyVisible {
            print("ARViewCtrl: app will enter foreground!")
            // session.run()
            //reloadEngineSession("", "", "")
            Context.inst().reloadARSession("", "", "")
        }
    }
    
    @objc func orientationDidChange(notification: NSNotification) {
        Logger.info("ARViewCtrl: orientation change ignored.")
/*
        guard let deviceOrientation = UIDevice.current.orientation as UIDeviceOrientation? else {
            return
        }

        // Force layout update
        view.setNeedsLayout() // not working
        view.layoutIfNeeded() // not working
        
        let width = Renderer.instance.viewportSize.width
        let height = Renderer.instance.viewportSize.height
        // Display_setSize(Int(width), Int(height))
        Display_setSize(Int(height), Int(width)) // swap the value because they haven't updated yet
        
        Renderer.instance.backgroundPass.setViewportChanged()
*/
    }
    
    func requestGeolocationUpdate() {
        Context.inst().locationManager().onRequestGeolocatinUpdate()
    }
    
    func moveToExtMediaFile(_ args: String) {
        mExtMediaStore.moveToExtMediaFile(args)
    }
    
    func shareExtMediaFile(_ args: String) {
        mExtMediaStore.shareExtMediaFile(self, args)
    }

}
