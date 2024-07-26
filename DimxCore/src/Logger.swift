//
//  Logger.swift
//  dimx-ios-app
//
//  Created by Sergii Romanov on 30/04/2023.
//  Copyright © 2023 Dimensions. All rights reserved.
//

import Foundation
import DimxNative

public class Logger {
    static private let instance = Logger()
    
    static public func debug(_ msg: String) {
        Logger_debug(msg)
    }
    
    static public func info(_ msg: String) {
        Logger_info(msg)
    }
    
    static public func warn(_ msg: String) {
        Logger_warn(msg)
    }
    
    static public func error(_ msg: String) {
        Logger_error(msg)
    }
}
