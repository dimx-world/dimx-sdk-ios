//
//  AnchorSession.swift
//  dimx-ios-app
//
//  Created by Sergii Romanov on 29/07/2022.
//  Copyright © 2022 Dimensions. All rights reserved.
//

import Foundation
import ARCore
import DimxNative
func cloudAnchorStateToCore(_ state: GARCloudAnchorState) -> Int {
    switch (state) {
        case .taskInProgress: return Int(CLOUD_ANCHOR_STATE_IN_PROGRESS)
        case .success: return Int(CLOUD_ANCHOR_STATE_SUCCESS)
        case .errorInternal: fallthrough
        case .errorNotAuthorized: fallthrough
        case .errorResourceExhausted: fallthrough
        case .errorHostingDatasetProcessingFailed: fallthrough
        case .errorCloudIdNotFound: fallthrough
        case .errorResolvingSdkVersionTooOld: fallthrough
        case .errorResolvingSdkVersionTooNew: fallthrough
        case .errorHostingServiceUnavailable:
            return Int(CLOUD_ANCHOR_STATE_ERROR)
        case .none: return Int(CLOUD_ANCHOR_STATE_NONE)
        default:
            return Int(CLOUD_ANCHOR_STATE_NONE)
    };
}

func cloudAnchorStateToStr(_ state: GARCloudAnchorState) -> String {
    switch (state) {
        case .taskInProgress:                       return "In Progress"
        case .success:                              return "Success"
        case .errorInternal:                        return "Internal Error"
        case .errorNotAuthorized:                   return "Not Authorized"
        case .errorResourceExhausted:               return "Resource Exhausted"
        case .errorHostingDatasetProcessingFailed:  return "Hosting Dataset Processing Failed"
        case .errorCloudIdNotFound:                 return "Cloud Id Not Found"
        case .errorResolvingSdkVersionTooOld:       return "Resolving Sdk Version Too Old"
        case .errorResolvingSdkVersionTooNew:       return "Resolving Sdk Version Too New"
        case .errorHostingServiceUnavailable:       return "Hosting Service Unavailable"
        case .none:                                 return "None"
        default:                                    return "None"
    };
}

func featureMapQualityToCore(_ quality: GARFeatureMapQuality) -> Int {
    switch (quality) {
        case .good:         return Int(FEATURE_MAP_QUALITY_GOOD)
        case .insufficient: return Int(FEATURE_MAP_QUALITY_INSUFFICIENT)
        case .sufficient:   return Int(FEATURE_MAP_QUALITY_SUFFICIENT)
        default:            return Int(FEATURE_MAP_QUALITY_NONE)
    }
}

func writeStringToPointer(_ string: String, _ pointer: UnsafeMutableRawPointer, _ bufferSize: Int) {
    let data = string.data(using: .utf8)!
    let bytesToWrite = min(bufferSize - 1, data.count) // Reserve one byte for null terminator
    data.withUnsafeBytes { (bytes: UnsafeRawBufferPointer) in
        pointer.copyMemory(from: bytes.baseAddress!, byteCount: bytesToWrite)
    }
    // Write null terminator
    pointer.advanced(by: bytesToWrite).storeBytes(of: UInt8(0), as: UInt8.self)
}

class GAnchorInfo {
    var token: UInt = 0
    var cloudAnchor: GARAnchor?  // used when resolved
    var localAnchorId: Int = -1  // used when hosted
    var hostFuture: GARHostCloudAnchorFuture?
    var resolveFuture: GARResolveCloudAnchorFuture?
}

class AnchorSession
{
    let ARCORE_API_KEY = "AIzaSyC9CB2bvJ6iZN1YruOlc09-7nlMMB2NucA"
    static let instance = AnchorSession()

    var gSession: GARSession! = nil
    var anchors = [GAnchorInfo?]()

    static func initCallbacks() {
        g_swiftCloudAnchorSession().pointee.initialize = {
            () -> () in AnchorSession.instance.initialize()
        }
        g_swiftCloudAnchorSession().pointee.createAnchor = {
            (token: UInt, transformPtr: Optional<UnsafeRawPointer>, ttlDays: Int) in
                AnchorSession.instance.createAnchor(token, transformPtr!, ttlDays)
        }
        g_swiftCloudAnchorSession().pointee.resolveAnchor = {
            (token: UInt, nativeId: UnsafePointer<CChar>!) in
                AnchorSession.instance.resolveAnchor(token, String(cString: nativeId))
        }
        g_swiftCloudAnchorSession().pointee.removeAnchor = {
            (token: UInt, anchorId: Int) in AnchorSession.instance.removeAnchor(token, anchorId)
        }
/*
        g_swiftAnchorSession().pointee.getAnchorState = {
            (anchorId: Int) -> Int in return AnchorSession.instance.getAnchorState(anchorId)
        }
        g_swiftAnchorSession().pointee.getAnchorStateStr = {
            (anchorId: Int, outStr: Optional<UnsafeMutableRawPointer>, bufferSize: Int) -> () in AnchorSession.instance.getAnchorStateStr(anchorId, outStr!, bufferSize)
        }
*/
        g_swiftCloudAnchorSession().pointee.getAnchorTracking = {
            (anchorId: Int, outPtr: Optional<UnsafeMutableRawPointer>) in
                AnchorSession.instance.getAnchorTracking(anchorId, outPtr!)
        }
        g_swiftCloudAnchorSession().pointee.featureMapQuality = {
            () -> Int in return AnchorSession.instance.featureMapQuality()
        }
    }

    func initialize() {
        do {
            let config = GARSessionConfiguration()
            config.cloudAnchorMode = .enabled
            gSession = try GARSession(apiKey: ARCORE_API_KEY, bundleIdentifier: nil)
            var error: NSError?
            if let session = gSession {
                session.setConfiguration(config, error: &error)
                if let error = error {
                    Logger.error("Error setting GARSession configuration: \(error)")
                }
            } else {
                Logger.error("Error: gSession is nil")
            }
        } catch {
            fatalError("Error creating GARSession: \(error.localizedDescription)")
        }
    }
    
    func update() {
        if DeviceAR.instance.currentFrame() != nil {
            do {
                try gSession!.update(DeviceAR.instance.currentFrame()!)
            } catch {
                Logger.error("Error updating GARSession: \(error.localizedDescription)")
            }
        }
    }

    func createAnchor(_ token: UInt, _ transformPtr: UnsafeRawPointer, _ ttlDays: Int) {
        let localAnchorId = DeviceAR.instance.createAnchor(transformPtr)
        let localAnchor = DeviceAR.instance.getAnchor(localAnchorId)
        
        let anchorId = anchors.count
        anchors.append(GAnchorInfo())
        anchors[anchorId]!.token = token
        anchors[anchorId]!.localAnchorId = localAnchorId
        
        do {
            weak var weakSelf = self
            anchors[anchorId]!.hostFuture = try gSession.hostCloudAnchor((localAnchor?.anchor)!, ttlDays: ttlDays,
                completionHandler: { (cloudId, state) in weakSelf?.onHostAnchor(token, cloudId, state, anchorId) })
        } catch {
            Logger.error("Failed to host cloud anchor: \(error.localizedDescription)")
            onHostAnchor(token, nil, .errorInternal, anchorId)
            //writeStringToPointer("Failed to host cloud anchor: \(error.localizedDescription)", outError, bufferSize)
        }
    }
    
    func resolveAnchor(_ token: UInt, _ nativeId: String) {
        let anchorId = anchors.count
        anchors.append(GAnchorInfo())
        anchors[anchorId]!.token = token
        
        do {
            weak var weakSelf = self
            anchors[anchorId]!.resolveFuture = try gSession.resolveCloudAnchor(nativeId,
                completionHandler: { (anchor, state) in weakSelf?.onResolveAnchor(token, anchor, state, anchorId) })
        }catch {
            Logger.error("\(error.localizedDescription) Failed to resolve cloud anchor: \(error.localizedDescription)");
            onResolveAnchor(token, nil, .errorInternal, anchorId)
        }
    }
    
    func removeAnchor(_ token: UInt, _ anchorId: Int) {
        if anchorId >= anchors.count {
            fatalError("Invalid local anchor id - out of bounds: \(anchorId)")
        }

        let index = anchorId >= 0 ? anchorId : anchors.firstIndex(where: {info in info?.token == token})
        if index == nil || index! < 0 {
            Logger.error("Failed to remove anchor: token \(token), anchorId \(anchorId), index \(String(describing: index))")
            return
        }

        if anchors[index!]!.resolveFuture != nil {
            anchors[index!]!.resolveFuture!.cancel()
        }

        if anchors[index!]!.hostFuture != nil {
            anchors[index!]!.hostFuture!.cancel()
        }
        
        if anchors[index!]!.localAnchorId >= 0 {
            DeviceAR.instance.deleteAnchor(anchors[index!]!.localAnchorId)
        }

        if anchors[index!]!.cloudAnchor != nil {
            gSession.remove(anchors[index!]!.cloudAnchor!)
        }

        anchors[index!] = nil
    }
    
    func onHostAnchor(_ token: UInt, _ cloudId: String?, _ state: GARCloudAnchorState, _ anchorId: Int) {
        if anchorId >= anchors.count {
            fatalError("Invalid anchor id - out of bounds: \(anchorId)")
        }

        if anchors[anchorId] == nil {
            fatalError("onHostAnchor: nil anchor info, idx = \(anchorId)")
        }

        anchors[anchorId]!.hostFuture = nil

        CloudAnchorSession_onCreateAnchor(token, cloudAnchorStateToStr(state), cloudId != nil ? cloudId! : "", cloudId != nil ? anchorId : -1)

        if cloudId == nil {
            removeAnchor(token, anchorId)
        }
    }

    func onResolveAnchor(_ token: UInt, _ anchor: GARAnchor?, _ state: GARCloudAnchorState, _ anchorId: Int) {
        if anchorId >= anchors.count {
            fatalError("Invalid anchor id - out of bounds: \(anchorId)")
        }
        
        if anchors[anchorId] == nil {
//            fatalError("onResolveAnchor: nil anchor info, idx = \(anchorId)")
            Logger.warn("Anchor resolved after removal [\(anchorId)]")
            if anchor != nil {
                gSession.remove(anchor!)
            }
            return
        }

        anchors[anchorId]!.cloudAnchor = anchor

        CloudAnchorSession_onResolveAnchor(token, anchor != nil ? anchorId : -1, cloudAnchorStateToStr(state))

        if anchor == nil {
            removeAnchor(token, anchorId)
        }
    }
/*
    func getAnchorStateStr(_ anchorId: Int, _ outStr: UnsafeMutableRawPointer, _ bufferSize: Int) {
        var result = ""
        if anchorId >= anchors.count {
            result = "LOCAL_ID_OUT_OF_BOUNDS"
        } else if anchors[anchorId] == nil {
            result = "LOCAL_ANCHOR_DELETED"
        } else if anchors[anchorId]!.anchor == nil {
            result = "NULL_CLOUD_ANCHOR"
        } else {
            result = cloudAnchorStateToStr(anchors[anchorId]!.anchor!.cloudState)
        }
        writeStringToPointer(result, outStr, bufferSize)
    }
*/
    func getAnchorTracking(_ anchorId: Int, _ outPtr: UnsafeMutableRawPointer) {
        if anchorId >= anchors.count {
            fatalError("Invalid anchor id - out of bounds")
        }
        if anchors[anchorId]!.cloudAnchor != nil {
            if anchors[anchorId]!.cloudAnchor!.trackingState == .tracking &&
               anchors[anchorId]!.cloudAnchor!.hasValidTransform
            {
                var tmp = anchors[anchorId]!.cloudAnchor!.transform
                TrackingResult_assign(outPtr, true, &tmp)
            }
        } else if anchors[anchorId]!.localAnchorId >= 0 {
            DeviceAR.instance.getAnchorTracking(anchors[anchorId]!.localAnchorId, outPtr)
        }
    }
    
    func featureMapQuality() -> Int {
        do {
            let transform = DeviceAR.instance.currentFrame()?.camera.transform
            if transform == nil {
                return Int(FEATURE_MAP_QUALITY_NONE)
            }
            let quality = try gSession.estimateFeatureMapQualityForHosting(transform!)
            return featureMapQualityToCore(quality)
        } catch {
            return Int(FEATURE_MAP_QUALITY_NONE)
        }
    }

}
