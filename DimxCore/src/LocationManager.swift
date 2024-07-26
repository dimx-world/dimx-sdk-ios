//
//  LocationManager.swift
//  dimx-ios-app
//
//  Created by Sergii Romanov on 03/08/2022.
//  Copyright © 2022 Dimensions. All rights reserved.
//

import Foundation
import CoreLocation
import DimxNative

class LocationManager: NSObject, CLLocationManagerDelegate {
    private let mManager = CLLocationManager()
    private var mLocation: CLLocation?
    private var mAuthCallback: ((Bool) -> Void)?

    override init() {
        super.init()
        mManager.delegate = self
        mManager.desiredAccuracy = kCLLocationAccuracyBest
        mManager.distanceFilter = 1.0
        mManager.startUpdatingLocation()
    }
    
    func location() -> CLLocation? {
        return mLocation;
    }

    func locationManager(_ manager: CLLocationManager, didChangeAuthorization status: CLAuthorizationStatus) {
        Logger.info("location manager authorization status changed: \(status.rawValue)")
        switch status {
            case .authorizedWhenInUse, .authorizedAlways:
                mAuthCallback?(true)
                mAuthCallback = nil
                break
            case .denied, .restricted:
                mAuthCallback?(false)
                mAuthCallback = nil
                break
            case .notDetermined:
                break
            @unknown default:
                break
        }
    }

    func locationManager(_ manager: CLLocationManager, didFailWithError error: Error) {
        Logger.error("location manager error: \(error.localizedDescription)")
    }
    
    func locationManager(_ manager: CLLocationManager, didUpdateLocations locations: [CLLocation]) {
        if let loc = locations.last {
            mLocation = loc
            //Logger.info("SWIFT Geolocation update: \(mLocation!)")
            processGeolocationUpdate(mLocation!.coordinate.latitude,
                                     mLocation!.coordinate.longitude,
                                     mLocation!.altitude,
                                     mLocation!.horizontalAccuracy,
                                     mLocation!.verticalAccuracy)
            if let webCtrl = Context.inst().webViewCtrl() {
                webCtrl.onsGeolocationUpdate(mLocation!)
            }
        }
    }
    
    func onRequestGeolocatinUpdate() {
        //Logger.info("SWIFT onRequestGeolocatinUpdate: \(mLocation!)")
        if mLocation != nil {
            processGeolocationUpdate(mLocation!.coordinate.latitude,
                                     mLocation!.coordinate.longitude,
                                     mLocation!.altitude,
                                     mLocation!.horizontalAccuracy,
                                     mLocation!.verticalAccuracy)
        }
    }

    func requestPermission(_ callback: @escaping (Bool) -> Void) {
        if !CLLocationManager.locationServicesEnabled() {
            callback(false)
            return
        }

        switch CLLocationManager.authorizationStatus() {
        case .notDetermined:
            mAuthCallback = callback
            mManager.requestWhenInUseAuthorization()
            return
        case .authorizedWhenInUse, .authorizedAlways:
            callback(true)
            return
        case .denied, .restricted:
            callback(false)
            return
        @unknown default:
            break
        }
        callback(false)
    }
}
