import Foundation
import ARKit
import DimxNative

var g_colors: [simd_float4] = [
    simd_float4(0.0, 0.0, 1.0, 1),
    simd_float4(0.0, 1.0, 0.0, 1),
    simd_float4(0.0, 1.0, 1.0, 1),
    simd_float4(1.0, 0.0, 0.0, 1),
    simd_float4(1.0, 0.0, 1.0, 1),
    simd_float4(1.0, 1.0, 0.0, 1),
    simd_float4(0.0, 0.0, 0.5, 1),
    simd_float4(0.0, 0.5, 0.0, 1),
    simd_float4(0.0, 0.5, 0.5, 1),
    simd_float4(0.5, 0.0, 0.0, 1),
    simd_float4(0.5, 0.0, 0.5, 1),
    simd_float4(0.5, 0.5, 0.0, 1)
]

class AnchorInfo {
    var image: ARReferenceImage?
    var imageAnchor: ARImageAnchor?
    var anchor: ARAnchor?
    var isTracked = false
    var transform = simd_float4x4()
    var width: Float =  1.0
    var height: Float = 1.0
}

class PlaneInfo {
    var anchor : ARPlaneAnchor?
    var color = simd_float4(1, 1, 1, 1)
}

class DeviceAR: NSObject, ARSessionDelegate
{
    static /*private*/ let instance = DeviceAR()
    override private init() {}
    
    private let session = ARSession()
    private var mCurrentFrame: ARFrame?
    private let configuration = ARWorldTrackingConfiguration()

    private var anchors = [AnchorInfo?]()
    
    private var mCameraMinZ: Float = 0.0
    private var mCameraMaxZ: Float = 0.0

    static func initCallbacks() {
        g_swiftDeviceAR().pointee.initialize = {
            (configPtr: Optional<UnsafeRawPointer>) -> () in
            DeviceAR.instance.initialize(configPtr: configPtr!)
        }
        g_swiftDeviceAR().pointee.postInit = {
            (configPtr: Optional<UnsafeRawPointer>) -> () in
            DeviceAR.instance.postInit(configPtr: configPtr!)
        }
        g_swiftDeviceAR().pointee.preFrameUpdate = {
            (frameContextPtr: Optional<UnsafeRawPointer>) -> () in
            DeviceAR.instance.preFrameUpdate(frameContextPtr: frameContextPtr!)
        }
        g_swiftDeviceAR().pointee.inFrameUpdate = {
            (frameContextPtr: Optional<UnsafeRawPointer>) -> () in
            DeviceAR.instance.inFrameUpdate(frameContextPtr: frameContextPtr!)
        }
        g_swiftDeviceAR().pointee.createMarker = {
            (markerPtr: Optional<UnsafeRawPointer>) -> Int in
            DeviceAR.instance.createMarker(markerPtr: markerPtr!)
        }
        g_swiftDeviceAR().pointee.deleteMarker = {
            (id: Int) -> () in
             DeviceAR.instance.deleteMarker(id: id)
        }
        g_swiftDeviceAR().pointee.createAnchor = {
            (transformPtr: Optional<UnsafeRawPointer>) -> Int in
            DeviceAR.instance.createAnchor(transformPtr!)
        }
        g_swiftDeviceAR().pointee.getAnchorTracking = {
            (id: Int, outPtr: Optional<UnsafeMutableRawPointer>) -> () in
            DeviceAR.instance.getAnchorTracking(id, outPtr!)
        }
        g_swiftDeviceAR().pointee.deleteAnchor = {
            (id: Int) -> () in
             DeviceAR.instance.deleteAnchor(id)
        }
        g_swiftDeviceAR().pointee.scanQRCode = {
            (outBuffer: Optional<UnsafeMutableRawPointer>, bufSize: Int) -> () in
            DeviceAR.instance.scanQRCode(outBuffer: outBuffer!, bufSize: bufSize)
        }
        g_swiftDeviceAR().pointee.raycast = {
            (origX: Float, origY: Float, origZ: Float, dirX: Float, dirY: Float, dirZ: Float, flags: UInt, outPtr: Optional<UnsafeMutableRawPointer>) -> () in
            DeviceAR.instance.raycast(simd_float3(origX, origY, origZ), simd_float3(dirX, dirY, dirZ), flags, outPtr!)
        }
    }

    func initialize(configPtr: UnsafeRawPointer) {
        Logger.info("DeviceAR initalize")
        mCameraMinZ = Config_getFloat(getEngineConfig(), "camera.minz")
        mCameraMaxZ = Config_getFloat(getEngineConfig(), "camera.maxz")
        
        configuration.maximumNumberOfTrackedImages = 10
        if Settings_iosPlaneDetection() {
            configuration.planeDetection = [.horizontal, .vertical]
        }
        configuration.automaticImageScaleEstimationEnabled = true
//        configuration.isLightEstimationEnabled = true

        session.delegate = self
        session.run(configuration)
    }

    func postInit(configPtr: UnsafeRawPointer) {
    }

    func pauseSession() {
        session.pause()
        mCurrentFrame = nil
        if (Renderer.instance.backgroundPass != nil) {
            Renderer.instance.backgroundPass.resetTexture()
        }
    }

    func restartSession() {
        pauseSession()
        configuration.detectionImages.removeAll()
        session.run(configuration, options: [.resetTracking, .removeExistingAnchors])
    }
    
    func preFrameUpdate(frameContextPtr: UnsafeRawPointer) {
        if (mCurrentFrame != nil) {
            var projMat = mCurrentFrame!.camera.projectionMatrix(for: Context.inst().getInterfaceOrientation(), viewportSize: Renderer.instance.viewportSize, zNear: CGFloat(mCameraMinZ), zFar: CGFloat(mCameraMaxZ))
            Camera_setProjectionMat(&projMat)
            var viewMat = mCurrentFrame!.camera.viewMatrix(for: Context.inst().getInterfaceOrientation())
            Camera_setViewMat(&viewMat)
        }
    }

    func inFrameUpdate(frameContextPtr: UnsafeRawPointer) {
        if mCurrentFrame == nil {
            return
        }
        if Settings_displayDebugLines() {
            for anchor in mCurrentFrame!.anchors {
                if anchor.isKind(of: ARImageAnchor.self) {
                    let img = anchor as! ARImageAnchor
                    Renderer.instance.debugPass.drawRect(Float(img.referenceImage.physicalSize.width),
                                                         Float(img.referenceImage.physicalSize.height),
                                                         img.transform,
                                                         img.isTracked ? simd_float4(0, 1, 0, 1) : simd_float4(1, 0, 0, 1))
                }
            }

            if Settings_displayPointCloud() {
                if mCurrentFrame!.rawFeaturePoints != nil {
                    for point in mCurrentFrame!.rawFeaturePoints!.points {
                        Renderer.instance.debugPass.drawPoint(point, 0.03, simd_float4(1.0, 1.0, 0.0, 1))
                    }
                }
            }

            if Settings_displayDebugPlanes() {
                var planeColorIdx = 0
                for anchor in mCurrentFrame!.anchors {
                    if anchor.isKind(of: ARPlaneAnchor.self) {
                        let plane = anchor as! ARPlaneAnchor
                        var color = g_colors[planeColorIdx % g_colors.count]
                        color.w = 0.4
                        Renderer.instance.debugPass.drawPlane(plane.geometry, plane.transform, color)
                        planeColorIdx += 1
                    }
                }
            }
        }
        
        do {
            try AnchorSession.instance.gSession!.update(mCurrentFrame!)
        } catch {
            Logger.error("Error updating GARSession: \(error.localizedDescription)")
        }
    }

    func currentFrame() -> ARFrame? {
        return mCurrentFrame
    }
    
    func session(_ session: ARSession, didUpdate frame: ARFrame) {
        mCurrentFrame = frame
//        Logger.info("Hello")
//        let currentTransform = frame.camera.transform
    }

    func session(_ session: ARSession, cameraDidChangeTrackingState camera: ARCamera) {
        Logger.info("TRACKING STATE CHANGED: \(camera.trackingState)")
    }
    
    func session(_ session: ARSession, didUpdate anchors: [ARAnchor]) {
        for anchor in anchors {
            let imageAnchor = anchor as? ARImageAnchor
            if imageAnchor != nil {
                processDetectedImage(imageAnchor: imageAnchor!)
                continue
            }
        }
    }
    
    func session(_ session: ARSession, didRemove anchors: [ARAnchor]) {
    }
    
    func session(_ session: ARSession, didAdd anchors: [ARAnchor]) {
        for anchor in anchors {
            let imageAnchor = anchor as? ARImageAnchor
            if imageAnchor != nil {
                Logger.info("Added image anchor [\(imageAnchor!.referenceImage.name!)]")
                processDetectedImage(imageAnchor: imageAnchor!)
                continue
            }
        }
    }
    
    func processDetectedImage(imageAnchor: ARImageAnchor) {
        for info in anchors {
            if info?.image?.hash == imageAnchor.referenceImage.hash {
                info!.imageAnchor = imageAnchor
                info!.isTracked = imageAnchor.isTracked
                info!.transform = imageAnchor.transform
                break
            }
        }
    }

    func createMarker(markerPtr: UnsafeRawPointer) -> Int {
        let texPtr = Marker_image(markerPtr)
/*
        let texWidth = Texture_width(texPtr)
        let texHeight = Texture_height(texPtr)
        let data = UnsafeMutableRawPointer(mutating: Texture_imageData(texPtr)!)

        var pixelBuffer: CVPixelBuffer?

        // For 32RGBA: error -6680 which means "The buffer does not support the specified pixel format."
        //let cvResult = CVPixelBufferCreateWithBytes(nil, texWidth, texHeight, kCVPixelFormatType_32RGBA, data, texWidth * 4, nil, nil, nil, &pixelBuffer)
         
        let cvResult = CVPixelBufferCreate(nil, texWidth, texHeight, kCVPixelFormatType_32ARGB, nil, &pixelBuffer)
        if (cvResult != 0) {
            fatalError("CVPixelBufferCreate failed: \(cvResult)")
        }

        CVPixelBufferLockBaseAddress(pixelBuffer!, CVPixelBufferLockFlags(rawValue: 0))
        let baseAddress = CVPixelBufferGetBaseAddress(pixelBuffer!)!
        // RGBA -> ARGB + vertical flip
        for row in 0 ..< texHeight {
            for col in 0 ..< texWidth {
                let srcPixel = data + (texHeight-1 - row) * texWidth * 4 + col * 4
                let destPixel = baseAddress + row * texWidth * 4 + col * 4
                destPixel.copyMemory(from: srcPixel + 3, byteCount: 1) // copy Alpha
                (destPixel+1).copyMemory(from: srcPixel, byteCount: 3) // copy RGB
            }
        }
        CVPixelBufferUnlockBaseAddress(pixelBuffer!, CVPixelBufferLockFlags(rawValue: 0))

        let image = ARReferenceImage(pixelBuffer!, orientation: .up, physicalWidth: CGFloat(Marker_width(markerPtr)))
*/
        let fileData = Data(bytes: Texture_fileData(texPtr), count: Texture_fileDataSize(texPtr))
        //let uiimage = UIImage(named: "marker-stones.png")!
        let uiimage = UIImage(data: fileData)!
        
        let cgimage = uiimage.cgImage

        let image = ARReferenceImage.init(cgimage!, orientation: .up, physicalWidth: CGFloat(Marker_width(markerPtr)))

        image.name = String(cString: Marker_name(markerPtr))

        let id = anchors.count
        anchors.append(AnchorInfo())
        anchors[id]?.image = image
        anchors[id]?.width = Marker_width(markerPtr)
        anchors[id]?.height = Marker_height(markerPtr)
        
        configuration.detectionImages.insert(image)
        session.pause()
        session.run(configuration/*, options: [.resetTracking, .removeExistingAnchors]*/)

        return id
    }
    
    func deleteMarker(id: Int) {
        if id >= anchors.count {
            fatalError("Invalid image id - out of bounds")
        }
        if anchors[id]?.image == nil {
            fatalError("Marker already deleted")
        }

        configuration.detectionImages.remove((anchors[id]?.image)!)
        session.run(configuration)

        anchors[id] = nil
    }
    
    func createAnchor(_ transformPtr: UnsafeRawPointer) -> Int {
        let id = anchors.count
        anchors.append(AnchorInfo())
        var mat = simd_float4x4()
        Transform_matrix(transformPtr, &mat)
        anchors[id]!.anchor = ARAnchor(transform: mat)
        session.add(anchor: anchors[id]!.anchor!)
        return id
    }
    
    func getAnchor(_ id: Int) -> AnchorInfo? {
        return id < anchors.count ? anchors[id] : nil
    }
    
    func getAnchorTracking(_ id: Int, _ outPtr: UnsafeMutableRawPointer) {
        if id >= anchors.count {
            fatalError("Invalid image id - out of bounds")
        }
        if anchors[id]!.imageAnchor != nil {
            var tmp = anchors[id]!.transform
            TrackingResult_assign(outPtr, anchors[id]!.isTracked, &tmp)
            return
        }
        if anchors[id]!.image != nil {
            return // the marker was created but not detected yet
        }
        if anchors[id]!.anchor != nil {
            var tmp = anchors[id]!.anchor!.transform
            TrackingResult_assign(outPtr, true, &tmp)
            return
        }
        //fatalError("Invalid anchor id")
    }
    
    func deleteAnchor(_ id: Int) {
        if id >= anchors.count {
            fatalError("Invalid image id - out of bounds")
        }
        session.remove(anchor: anchors[id]!.anchor!)
        anchors[id] = nil
    }
    
    func scanQRCode(outBuffer: UnsafeMutableRawPointer, bufSize: Int) {
        let pixelBuffer = mCurrentFrame?.capturedImage;
        if (pixelBuffer != nil) {
            CVPixelBufferLockBaseAddress(pixelBuffer!, CVPixelBufferLockFlags.readOnly)
            // The captured image comes in YCbCr format (also called YUV420)
            // We take the first plane (luminance) as it give as a grayscale image
            Vision_decodeQRCode(CVPixelBufferGetWidthOfPlane(pixelBuffer!, 0),
                                CVPixelBufferGetHeightOfPlane(pixelBuffer!, 0),
                                CVPixelBufferGetBytesPerRowOfPlane(pixelBuffer!, 0),
                                0, // ImageFormat - always ImageFormat::YUV420
                                CVPixelBufferGetBaseAddressOfPlane(pixelBuffer!, 0),
                                outBuffer, bufSize);
            CVPixelBufferUnlockBaseAddress(pixelBuffer!, CVPixelBufferLockFlags.readOnly)
        }
    }
    
    func raycast(_ orig: simd_float3, _ dir: simd_float3, _ flags: UInt, _ outPtr: UnsafeMutableRawPointer) {
        let target = raycastStrategyFromCore(Settings_iosRaycastStrategy())
        let query = ARRaycastQuery(origin: orig, direction: dir, allowing: target, alignment: .any)
        let results = session.raycast(query)
        if (results.count > 0) {
            let idx = 0 //(flags & SWIFT_AR_RAYCAST_FURTHEST_FIRST == 0) ? 0 : results.count - 1
            var pos = simd_float3(results[idx].worldTransform.columns.3.x,
                                  results[idx].worldTransform.columns.3.y,
                                  results[idx].worldTransform.columns.3.z)
            var norm = simd_float3(results[idx].worldTransform.columns.1.x,
                                   results[idx].worldTransform.columns.1.y,
                                   results[idx].worldTransform.columns.1.z)
            RaycastResult_assign(outPtr, true, &pos, &norm)
        }
    }
    
}
