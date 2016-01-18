cc_library(
    name = "wild",
    srcs = [
        "atom.cpp",
        "adler32.cpp",
        "exception.cpp",
        "module.cpp",
        "string.cpp",
        "io.cpp",
        "Signaler.cpp",
        "stacktrace.cpp",
    ],
    hdrs = [
        "atom.hpp",
        "AppendList.hpp",
        "BlockingQueue.hpp",
        "compat.hpp",
        "Fd.hpp",
        "io.hpp",
        "IdAllocator.hpp",
        "types.hpp",
        "errno.hpp",
        "adler32.hpp",
        "exception.hpp",
        "likely.hpp",
        "module.hpp",
        "module_init_failed.hpp",
        "ScopeGuard.hpp",
        "string.hpp",
        "unique_name.hpp",
        "Signaler.hpp",
        "SpinLock.hpp",
        "with_lock.hpp",
        "ForwardList.hpp",
        "FreeList.hpp",
        "SharedAny.hpp",
        "utility.hpp",
        "stacktrace.hpp",
        "SharedSpinLock.hpp",
    ],
    copts = [
        "-std=c++1y",
        "-Wall",
        "-Wextra",
        "-Werror",
        "-Wconversion",
        "-Wshadow",
    ],
    visibility = ["//visibility:public"],
)
