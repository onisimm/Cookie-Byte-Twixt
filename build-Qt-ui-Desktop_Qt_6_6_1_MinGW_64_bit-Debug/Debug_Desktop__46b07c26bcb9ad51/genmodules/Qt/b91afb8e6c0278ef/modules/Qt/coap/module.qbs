import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "Coap"
    Depends { name: "Qt"; submodules: ["core"]}

    architectures: ["x86_64"]
    targetPlatform: "windows"
    hasLibrary: true
    staticLibsDebug: []
    staticLibsRelease: []
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt6Coap"
    libNameForLinkerRelease: "Qt6Coap"
    libFilePathDebug: undefined
    libFilePathRelease: "C:/Qt/6.6.1/mingw_64/lib/libQt6Coap.a"
    pluginTypes: []
    moduleConfig: ["shared"]
    cpp.defines: ["QT_COAP_LIB"]
    cpp.systemIncludePaths: ["C:/Qt/6.6.1/mingw_64/include","C:/Qt/6.6.1/mingw_64/include/QtCoap"]
    cpp.libraryPaths: []
    Group {
        files: [Qt["coap"].libFilePath]
        filesAreTargets: true
        fileTags: ["dynamiclibrary_import"]
    }
}
