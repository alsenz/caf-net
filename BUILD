cc_library(
    name = "caf-net",
    srcs = glob(["src/*.cpp"]),
    hdrs = glob(["src/*.hpp"]),
    copts = ["-std=c++17"],
    linkopts = ["-lstdc++fs"],
    visibility = ["//visibility:public"],
    deps = [
        "@common",
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
        ":caf-net",
        "@gtest//:gtest_main",
    ],
)
