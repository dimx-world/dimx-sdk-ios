# DimXCore

## Steps to setup:
- Build .a static library
- Create .xcframework with 
    `xcodebuild -create-xcframework -library libdimx-core.a -output NativeCore.xcframework`
- Add to `Package.swift` as `.binaryTarget(name: "NativeCore", path: "Libs/NativeCore/NativeCore.xcframework")`
- Add to `Package.swift` as a dependency for `DimxCore`
- Add files to `Libs` folder

## Troubleshooting:
- Error could be solved by deleting `CONFIGURATION_BUILD_DIR` param in build settings. It should be used from project, not from specific target.
```
Showing All Messages
the path does not point to a valid library: /Users/andrey/Developer/DimX/ios-env/install/xcode/libdimx-core.a
```
