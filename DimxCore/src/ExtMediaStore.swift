//
//  ExtMediaStore.swift
//  DimxCore
//
//  Created by Sergii Romanov on 01/07/2024.
//  Copyright © 2024 Dimensions. All rights reserved.
//

import Foundation
import UIKit
import Photos

class ExtMediaStore
{
    private var mLocalIdentifiers = [String: String]()

    func moveToExtMediaFile(_ args: String) {
        let parts = args.split(separator: ";")
        if parts.count != 2 {
            Logger.error("moveToExtMediaFile: invalid args [\(args)] [\(parts.count)] [\(parts[0])]")
            return
        }
        let sourcePath = String(parts[0])
        let destPath = String(parts[1])
        
        PHPhotoLibrary.requestAuthorization { status in
            guard status == .authorized else {
                Logger.error("Save to media not authorized")
                DispatchQueue.main.async {
                    self.showAlert("Unable to save media file. Please grant access to Photos in the Settings and try again.")
                }
                return
            }

            PHPhotoLibrary.shared().performChanges({
                guard let creationRequest = self.makeCreationRequest(sourcePath) else {
                    Logger.error("ExtMediaStore: invalid creation request for file [\(sourcePath)]")
                    return
                }
                let assetPlaceholder = creationRequest.placeholderForCreatedAsset
                self.mLocalIdentifiers[destPath] = assetPlaceholder?.localIdentifier
            }) { success, error in
                if success {
                    Logger.info("ExtMediaStore: file saved [\(destPath)]")
                    do {
                        try FileManager.default.removeItem(at: URL(fileURLWithPath: sourcePath))
                        Logger.info("ExtMediaStore: file removed from documents [\(sourcePath)]")
                    } catch {
                        Logger.error("ExtMediaStore: failed to remove file from documents [\(sourcePath)]")
                    }
                } else if let error = error {
                    Logger.error("ExtMediaStore: failed to save file to library [\(sourcePath)]. Error: \(error.localizedDescription)")
                }
            }
        }
    }
    
    func shareExtMediaFile(_ viewCtrl: UIViewController, _ file: String) {
        guard let assetIdentifier = mLocalIdentifiers[file] else {
            Logger.error("shareExtMediaFile: unknown file [\(file)]")
            return
        }
        
        let assets = PHAsset.fetchAssets(withLocalIdentifiers: [assetIdentifier], options: nil)
        guard let asset = assets.firstObject else {
            Logger.error("shareExtMediaFile: failed to fetch asset for file [\(file)] localId [\(assetIdentifier)]")
            return
        }
        if isVideoFile(file) {
            shareVideoAsset(viewCtrl, asset)
        } else {
            shareImageAsset(viewCtrl, asset)
        }
    }
    
    private func isVideoFile(_ filePath: String) -> Bool {
        return filePath.hasSuffix(".mp4")
    }
    
    private func makeCreationRequest(_ filePath: String) -> PHAssetChangeRequest? {
        guard let url = URL(string: filePath) else {
            Logger.error("ExtMediaStore: makeCreationRequest nil url for [\(filePath)]")
            return nil }
        if isVideoFile(filePath) {
            return PHAssetChangeRequest.creationRequestForAssetFromVideo(atFileURL: url)
        }
        return PHAssetChangeRequest.creationRequestForAssetFromImage(atFileURL: url)
    }
    
    private func shareImageAsset(_ viewCtrl: UIViewController, _ asset: PHAsset) {
        let imageManager = PHImageManager.default()
        let options = PHImageRequestOptions()
        options.isSynchronous = true
        options.deliveryMode = .highQualityFormat
        
        imageManager.requestImageDataAndOrientation(for: asset, options: options) { (data, _, orientation, info) in
            if let data = data, let image = UIImage(data: data) {
                // Create the activity view controller
                let activityViewController = UIActivityViewController(activityItems: [image], applicationActivities: nil)
                
                // Present the activity view controller
                DispatchQueue.main.async {
                    viewCtrl.present(activityViewController, animated: true, completion: nil)
                }
            }
        }
    }
    
    private func shareVideoAsset(_ viewCtrl: UIViewController, _ asset: PHAsset) {
        let options = PHVideoRequestOptions()
        options.deliveryMode = .highQualityFormat

        PHImageManager.default().requestAVAsset(forVideo: asset, options: options) { (avAsset, audioMix, info) in
            if let urlAsset = avAsset as? AVURLAsset {
                let videoURL = urlAsset.url
                let activityViewController = UIActivityViewController(activityItems: [videoURL], applicationActivities: nil)

                DispatchQueue.main.async {
                    viewCtrl.present(activityViewController, animated: true, completion: nil)
                }
            }
        }
    }
    
    func showAlert(_ message: String) {
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
        alertController.addAction(UIAlertAction(title: "Continue", style: .default, handler: { /*[alertController]*/ action in
            switch action.style{
                case .default: break
                case .cancel: break
                case .destructive: break
                @unknown default: break
            }
        }))
        
        topController.present(alertController, animated: true, completion: nil)
    }
}
