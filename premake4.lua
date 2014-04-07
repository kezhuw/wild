solution "wild"
    configurations {"Debug", "Release"}
    location "build"
    flags {"ExtraWarnings", "FatalWarnings", "Symbols"}
    buildoptions {
        '-std=c++11',
        '-Wshadow',
        '-Wconversion',
        '-Wold-style-cast',
    }

project "wild"
    kind "SharedLib"
    language "C++"
    location "build"
    files {
        "adler32.cpp",
        "exception.cpp",
        "misc.cpp",
        "module.cpp",
        "string.cpp",
        "io.cpp",
    }
    links {
        "execinfo",
    }
