DLN_COPTS = [
    "-std=c++17",
    "-fconcepts",
    "-Wno-write-strings"
]

DLN_LINKOPTS = [
    "-lstdc++fs",
    "-lpthread"
]

DLN_DEPS = [
    "@common",
    "@libcaf",
    "@libr3",
    "@nlohmann//:json",
    "@node_http//:http_parser",
    "@url_cpp//:url-cpp"
]

cc_library(
    name = "dandelion-headers",
    hdrs = glob([
        "dandelion/*.hpp",
    ]),
    copts = DLN_COPTS,
    linkopts = DLN_LINKOPTS,
    visibility = ["//visibility:public"],
    deps = DLN_DEPS,
)

cc_library(
    name = "dandelion",
    srcs = glob(["src/*.cpp"]),
    copts = DLN_COPTS,
    linkopts = DLN_LINKOPTS,
    visibility = ["//visibility:public"],
    deps = DLN_DEPS + [":dandelion-headers"],
    strip_include_prefix = "src",
    include_prefix = "dandelion"
)

cc_test(
    name = "tests",
    srcs = glob(["tests/*.cpp"]),
    copts = DLN_COPTS,
    linkopts = DLN_LINKOPTS,
    deps = [
        ":dandelion",
        "@gtest//:gtest_main",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "lib-tests",
    srcs = glob(["tests/*.cpp"]),
    copts = DLN_COPTS,
    linkopts = DLN_LINKOPTS,
    deps = [
        ":dandelion",
        "@gtest//:gtest_main",
    ],
    visibility = ["//visibility:public"],
)

