import Foundation
import MetalKit
import DimxNative

class Vec2 {
    var x: Float = 0
    var y: Float = 0
    init() {
        x = 0
        y = 0
    }
    init(xx: Float, yy: Float) {
        x = xx
        y = yy
    }
}

func screenPos(_ point: CGPoint, _ bounds: CGRect) -> Vec2 {
    // UIScreen.main.scale)
    let scaleX = Renderer.instance.viewportSize.width / bounds.width
    let scaleY = Renderer.instance.viewportSize.height / bounds.height
    return Vec2(xx: Float(point.x * scaleX), yy: Float(point.y * scaleY))
}


class TouchListener {
  
    var count: Int = 0
    var firstFinger: UInt = 0
    var firstFingerPos = Vec2()
    
    func onTouchDown(_ key: UInt, _ pos: Vec2) {
        //Logger.debug("touch down: \(key) [\(pos.x) \(pos.y)]")
        if count == 0 {
            firstFinger = key
            firstFingerPos = pos
            processTouchDown(pos.x, pos.y)
        } else if count == 1 {
            processMultitouchDown(firstFingerPos.x, firstFingerPos.y)
        }
        count += 1
    }
    
    func onTouchMove(_ key: UInt, _ pos: Vec2) {
        if key == firstFinger {
            firstFingerPos = pos
            processTouchMove(pos.x, pos.y)
        }
    }

    func onTouchUp(_ key: UInt, _ pos: Vec2) {
        //Logger.debug("touch up: \(key) [\(pos.x) \(pos.y)]")
        count -= 1
        if count == 1 {
            processMultitouchUp(firstFingerPos.x, firstFingerPos.y)
        } else if count == 0 {
            processTouchUp(firstFingerPos.x, firstFingerPos.y)
        }
        
        // clean up
        if count <= 0 {
            cancelAll()
        }
    }
    
    func cancelAll() {
        //Logger.debug("touch cancel")
        count = 0
        firstFinger = 0
        firstFingerPos = Vec2()
    }
}

class ARView: MTKView, UIKeyInput
{
    let touchListener = TouchListener()
    
    override init(frame frameRect: CGRect, device: MTLDevice?) {
        super.init(frame: frameRect, device: device)
        isMultipleTouchEnabled = true
    }

    required init(coder: NSCoder) {
        super.init(coder: coder)
        isMultipleTouchEnabled = true
    }

    var hasText: Bool {
        return true
    }
    
    func insertText(_ text: String) {
        if (text.count > 0) {
            processInsertText(Int(text.unicodeScalars.first!.value))
        }
    }
    
    func deleteBackward() {
        processDeleteBackward()
    }
    
    override var canBecomeFirstResponder: Bool {
        return true
    }

    
    override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?) {
        super.touchesBegan(touches, with: event)
        for touch in touches {
            touchListener.onTouchDown(UInt(bitPattern: ObjectIdentifier(touch)),
                                      screenPos(touch.location(in: self), bounds))
        }
    }
    
    override func touchesMoved(_ touches: Set<UITouch>, with event: UIEvent?) {
        super.touchesMoved(touches, with: event)
        for touch in touches {
            touchListener.onTouchMove(UInt(bitPattern: ObjectIdentifier(touch)),
                                       screenPos(touch.location(in: self), bounds))
        }
    }
    
    override func touchesEnded(_ touches: Set<UITouch>, with event: UIEvent?) {
        super.touchesEnded(touches, with: event)
        //Logger.debug("BDS: \(bounds)")
        //Logger.debug("VPS: \(Renderer.instance.viewportSize)")
        //Logger.debug("SCL: \(UIScreen.main.scale)")
        for touch in touches {
            //let pnt = touch.location(in: self)
            //Logger.debug("PNT: \(pnt)")
            touchListener.onTouchUp(UInt(bitPattern: ObjectIdentifier(touch)),
                                    screenPos(touch.location(in: self), bounds))
        }
    }
    
    override func touchesCancelled(_ touches: Set<UITouch>, with event: UIEvent?) {
        super.touchesCancelled(touches, with: event)
        touchListener.cancelAll()
    }
}
