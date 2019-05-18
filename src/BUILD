cc_library(
    name = "net",
    srcs = glob(["*.cpp"]),
    hdrs = glob(["*.hpp"]),
    copts = ["-std=c++17"],
    linkopts = ["-lstdc++fs"],
    visibility = ["//visibility:public"],
    deps = [
        "//src/common",
        "@libcaf",
        "@libr3",
        "@nlohmann//:json",
        "@node_http//:http_parser",
        "@url_cpp//:url-cpp",
    ],
)

cc_test(
    name = "tests",
    srcs = glob(["tests/*.cpp"]),
    copts = ["-std=c++17"],
    linkopts = ["-lstdc++fs"],
    deps = [
        ":net",
        "@gtest//:gtest_main",
    ],
)
