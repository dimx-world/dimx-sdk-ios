import Foundation
import Metal
import DimxNative

class Texture
{
    var mTexture: MTLTexture!
    var mCoreTex: UnsafeRawPointer
    var mDescriptor = MTLTextureDescriptor()
    var mRenderPassDescriptor: MTLRenderPassDescriptor! // for render target
    //var mSamplerState: MTLSamplerState!
    
    static func initCallbacks() {
        g_swiftTexture().pointee.createTexture = { (coreTex: Optional<UnsafeRawPointer>) -> (Int) in return Texture.create(coreTex!) }
        g_swiftTexture().pointee.deleteTexture = { (id: Int) -> () in Texture.delete(id) }
        g_swiftTexture().pointee.updateImageData = {
            (_ id: Int, _ mipLevel: Int, _ width: Int, _ height: Int, _ data: Optional<UnsafeRawPointer>) -> () in
            Renderer.instance.textures[id]!.updateImageData(mipLevel, width, height, data!)
        }
    }

    static func create(_ coreTex: UnsafeRawPointer) -> Int {
        Renderer.instance.textures.append(Texture(coreTex))
        return Renderer.instance.textures.count - 1
    }

    static func delete(_ id: Int) {
        Renderer.instance.textures[id] = nil
    }
    
    init(_ coreTex: UnsafeRawPointer) {
        mCoreTex = coreTex
        mDescriptor.textureType = textureTypeFromCore(Texture_textureType(coreTex))
        mDescriptor.pixelFormat = pixelFormatFromCore(Texture_internalFormat(coreTex))
        //mDescriptor.mipmapLevelCount = 1
        if Texture_renderTarget(coreTex) {
            mDescriptor.usage = [.renderTarget, .shaderRead]
        }

        let width = Texture_width(coreTex)
        let height = Texture_height(coreTex)
        let numLods = Texture_numLods(coreTex)
        if width > 0 && height > 0 {
            createTexture(width, height)
            let imageData = Texture_imageData(coreTex)
            if imageData != nil {
                updateImageData(0, width, height, imageData!)
            }
        } else if numLods > 0 {
            mDescriptor.mipmapLevelCount = numLods
            for i in 0..<numLods {
                updateImageData(i, Texture_lodWidth(coreTex, i), Texture_lodHeight(coreTex, i), Texture_lodImageData(coreTex, i)!)
            }
        }
/*
        let samplerDescriptor = MTLSamplerDescriptor()
        samplerDescriptor.sAddressMode = .repeat
        samplerDescriptor.tAddressMode = .repeat
        samplerDescriptor.rAddressMode = .repeat
        samplerDescriptor.minFilter = .linear
        samplerDescriptor.magFilter = .linear
        samplerDescriptor.mipFilter = .linear
        mSamplerState = Renderer.instance.device.makeSamplerState(descriptor: samplerDescriptor)
*/
    }

    func createTexture(_ width: Int, _ height: Int) {
        mDescriptor.width = width
        mDescriptor.height = height
        mTexture = Renderer.instance.device.makeTexture(descriptor: mDescriptor)
        if Texture_renderTarget(mCoreTex) {
            mRenderPassDescriptor = MTLRenderPassDescriptor()
            mRenderPassDescriptor.colorAttachments[0].texture = mTexture
            mRenderPassDescriptor.colorAttachments[0].clearColor = MTLClearColor(red: 0.0, green: 0.0, blue: 0.0, alpha: 0.0)
            mRenderPassDescriptor.colorAttachments[0].loadAction = .clear
            mRenderPassDescriptor.colorAttachments[0].storeAction = .store
        }
    }
    
    func updateImageData(_ mipLevel: Int, _ width: Int, _ height: Int, _ data: UnsafeRawPointer) {
        if (mTexture == nil) {
            createTexture(width, height)
        }
        let bytesPerPixel = Texture_bytesPerPixel(mCoreTex)
        let bytesPerRow = bytesPerPixel * width
        let region = MTLRegionMake2D(0, 0, width, height)
        if mDescriptor.textureType == .typeCube {
            for faceIdx in 0..<6 {
                let dataOffset = bytesPerPixel * width * height * faceIdx
                mTexture.replace(region: region,
                                 mipmapLevel: mipLevel,
                                 slice: faceIdx,
                                 withBytes: data.advanced(by: dataOffset),
                                 bytesPerRow: bytesPerRow,
                                 bytesPerImage: 0)
            }
        } else {
            mTexture.replace(region: region,
                             mipmapLevel: mipLevel,
                             withBytes: data,
                             bytesPerRow: bytesPerRow)
        }
    }
}
