DLN_COPTS = [
    "-std=c++17",
    "-fconcepts",
    "-Wno-write-strings",
]

DLN_LINKOPTS = [
    "-lstdc++fs",
    "-lpthread",
]

DLN_DEPS = [
    "@common",
    "@libcaf",
    "@libr3",
    "@nlohmann//:json",
    "@node_http//:http_parser",
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
    include_prefix = "dandelion",
    linkopts = DLN_LINKOPTS,
    strip_include_prefix = "src",
    visibility = ["//visibility:public"],
    deps = DLN_DEPS + [":dandelion-headers"],
)

cc_test(
    name = "tests",
    srcs = glob(["tests/*.cpp"]),
    copts = DLN_COPTS,
    linkopts = DLN_LINKOPTS,
    visibility = ["//visibility:public"],
    deps = [
        ":dandelion",
        "@gtest//:gtest_main",
    ],
)

cc_library(
    name = "lib-tests",
    srcs = glob(["tests/*.cpp"]),
    copts = DLN_COPTS,
    linkopts = DLN_LINKOPTS,
    visibility = ["//visibility:public"],
    deps = [
        ":dandelion",
        "@gtest//:gtest_main",
    ],
)
