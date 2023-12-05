import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "Pdf"
    Depends { name: "Qt"; submodules: ["core","gui"]}

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
    libNameForLinkerDebug: "Qt6Pdf"
    libNameForLinkerRelease: "Qt6Pdf"
    libFilePathDebug: undefined
    libFilePathRelease: "C:/Qt/6.6.1/mingw_64/lib/libQt6Pdf.a"
    pluginTypes: []
    moduleConfig: ["shared"]
    cpp.defines: ["QT_PDF_LIB"]
    cpp.systemIncludePaths: ["C:/Qt/6.6.1/mingw_64/include","C:/Qt/6.6.1/mingw_64/include/QtPdf"]
    cpp.libraryPaths: []
    Group {
        files: [Qt["pdf"].libFilePath]
        filesAreTargets: true
        fileTags: ["dynamiclibrary_import"]
    }
}
