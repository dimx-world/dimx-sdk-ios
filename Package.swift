// swift-tools-version: 5.10
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "DimxWorld",
    platforms: [
        .iOS("16.4")
    ],
    products: [
        .library(
            name: "DimxCore",
            targets: ["DimxCore"]
        ),
    ],
    dependencies: [
        .package(url: "https://github.com/google-ar/arcore-ios-sdk", from: "1.44.0")
    ],
    targets: [
        .target(
            name: "DimxCore",
            dependencies: [
                "DimxNative",
                .product(name: "ARCoreGARSession", package: "arcore-ios-sdk"),
                .product(name: "ARCoreCloudAnchors", package: "arcore-ios-sdk"),
                .product(name: "ARCoreGeospatial", package: "arcore-ios-sdk"),
            ],
            path: "DimxCore",
            resources: [
                .copy("src/WebInterface.js"),
                .copy("data")
            ],
            linkerSettings: [
                .unsafeFlags(["-ObjC"])
            ]
        ),
        
        .target(
            name: "DimxNative",
            dependencies: [
                "dimx-core",
                "dimx-core-headers",
                "dimx-vision",
                "dimx-net",
                "dxaudio",
                "dxvideo",
                "quickjspp",
                "jsoncpp",
                "zstd",
                "ZXing",
                "ozz_animation",
                "ozz_base",
                //"ozz_geometry",
                //"ozz_options",
                "openal-soft",
                "avcodec",
                "avformat",
                "avutil",
                "swresample",
                "swscale"
            ],
            path: "DimxNative",
            sources: ["src"],
            publicHeadersPath: ".",
            cxxSettings: [
                .unsafeFlags(["-std=c++17"]),
                .define("DIMX_PLATFORM_IOS")
            ],
            linkerSettings: [
//                .linkedFramework("CoreAudio"),
                .linkedFramework("VideoToolbox"),
                .linkedLibrary("bz2"),
                .linkedLibrary("iconv")
            ]
        ),
        .binaryTarget(name: "dimx-core", path: "Libs/NativeCore/dimx-core.xcframework"),
        .binaryTarget(name: "dimx-core-headers", path: "Libs/NativeCore/dimx-core-headers.xcframework"),
        .binaryTarget(name: "dimx-vision", path: "Libs/NativeCore/dimx-vision.xcframework"),
        .binaryTarget(name: "dimx-net", path: "Libs/NativeCore/dimx-net.xcframework"),
        .binaryTarget(name: "dxaudio", path: "Libs/NativeCore/dxaudio.xcframework"),
        .binaryTarget(name: "dxvideo", path: "Libs/NativeCore/dxvideo.xcframework"),
        .binaryTarget(name: "quickjspp", path: "Libs/NativeCore/quickjspp.xcframework"),
        .binaryTarget(name: "jsoncpp", path: "Libs/NativeCore/jsoncpp.xcframework"),
        .binaryTarget(name: "zstd", path: "Libs/NativeCore/zstd.xcframework"),
        .binaryTarget(name: "ZXing", path: "Libs/NativeCore/ZXing.xcframework"),
        .binaryTarget(name: "ozz_animation", path: "Libs/NativeCore/ozz_animation.xcframework"),
        .binaryTarget(name: "ozz_base", path: "Libs/NativeCore/ozz_base.xcframework"),
//        .binaryTarget(name: "ozz_geometry", path: "Libs/NativeCore/ozz_geometry.xcframework"),
//        .binaryTarget(name: "ozz_options", path: "Libs/NativeCore/ozz_options.xcframework")
        .binaryTarget(name: "openal-soft", path: "Libs/External/openal-soft.xcframework"),
        .binaryTarget(name: "avcodec", path: "Libs/External/ffmpeg/avcodec.xcframework"),
        .binaryTarget(name: "avformat", path: "Libs/External/ffmpeg/avformat.xcframework"),
        .binaryTarget(name: "avutil", path: "Libs/External/ffmpeg/avutil.xcframework"),
        .binaryTarget(name: "swresample", path: "Libs/External/ffmpeg/swresample.xcframework"),
        .binaryTarget(name: "swscale", path: "Libs/External/ffmpeg/swscale.xcframework")
    ]
)
