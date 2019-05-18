genrule(
    name = "libr3_configure",
    srcs = glob(["**"]),
    outs = ["include/config.h"],
    cmd = "echo \"../../$(@D)/\" && cd external/libr3 && ./autogen.sh && ./configure && cp config.h ../../$(@D)/",  # It seems there is no way around this slowness until bazel gives us configure file facilities
)

cc_library(
    name = "libr3_config",
    hdrs = ["include/config.h"],  # Header only!,
)

cc_library(
    name = "libr3_3rdparty",
    srcs = glob(["3rdparty/*.c"], exclude = ["3rdparty/zmalloc.c"]),
    hdrs = glob(["3rdparty/*.h"]),
    strip_include_prefix = "3rdparty",
    deps = [":libr3_config"],
)

LIBR3_SRCS = glob(["src/*.c", "src/*.h"], exclude = ["src/json.c"])
LIBR3_SRCS.remove("src/gvc.c")  # No need for graphviz stuff
cc_library(
    name = "libr3",
    srcs = LIBR3_SRCS,
    hdrs = glob(["include/*.hpp", "include/*.h"], exclude = ["include/r3_json.h"]),
    deps = [":libr3_config", ":libr3_3rdparty", "@pcre"],
    visibility = ["//visibility:public"],
    #includes = ["$(location :libr3_config//config.h)"],
    includes = ["include"],
    copts = ["-DHAVE_STRDUP", "-DHAVE_STRNDUP"],  # This obviously isn't portable but the config doesn't seem to be included by the header
    #linkopts = ["-lpcre"],  # Also, this -lpcre is not so great, need to write a check for it! #TODO actually we allready need to kill this!
    strip_include_prefix = "include",  # Either need a way to strip src as well, or need a notion of public headers...
)
