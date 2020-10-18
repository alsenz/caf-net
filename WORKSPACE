load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")


# sole (for uuids!)
http_archive(
    name = "sole",
    build_file = "@//external:sole.BUILD",
    strip_prefix = "sole-653a25ad03775d7e0a2d50142160795723915ba6",
    urls = ["https://github.com/r-lyeh-archived/sole/archive/653a25ad03775d7e0a2d50142160795723915ba6.zip"],
)

# nlohmann
http_archive(
    name = "nlohmann",
    build_file = "@//external:nlohmann.BUILD",
    strip_prefix = "json-3.3.0",
    urls = ["https://github.com/nlohmann/json/archive/v3.3.0.tar.gz"],
)

# Google test
http_archive(
    name = "gtest",
    strip_prefix = "googletest-release-1.8.1",
    urls = ["https://github.com/google/googletest/archive/release-1.8.1.tar.gz"],
)

# Common
http_archive(
    name = "common",
    strip_prefix = "common-ec15fee37c7be312ab910d1aa09f4c4cb4ae26b3",
    urls = ["https://github.com/alsenz/common/archive/ec15fee.zip"],
)

# Caf dependency
# was 0.17.1
# TODO update to tag as soon as 0.18 released!
http_archive(
    name = "libcaf",
    build_file = "@//external:libcaf.BUILD",
    strip_prefix = "actor-framework-47ebe211f07d31725a3910ed966db1c31192d984",
    urls = ["https://github.com/actor-framework/actor-framework/archive/47ebe211f07d31725a3910ed966db1c31192d984.tar.gz"],
)

# libpcre; needed for r3
http_archive(
    name = "pcre",
    build_file = "@//external:pcre.BUILD",
    strip_prefix = "libpcre-pcre-8.36",
    urls = ["https://github.com/vmg/libpcre/archive/pcre-8.36.tar.gz"],
)

# R3 dependency
http_archive(
    name = "libr3",
    build_file = "@//external:libr3.BUILD",
    #sha256 = "db1fb91e51646e523e78b458643c0250231a2640488d5781109f95bd77c5eb82",
    #strip_prefix = "r3-1.3.4",
    strip_prefix = "r3-2.0",
    urls = ["https://github.com/c9s/r3/archive/2.0.tar.gz"],
)

# For caf-net also
http_archive(
    name = "url_cpp",
    build_file = "@//external:url-cpp.BUILD",
    strip_prefix = "url-cpp-e884eac2ab8ef3b4a93333dd79df97650fbb45bf",
    urls = ["https://github.com/seomoz/url-cpp/archive/e884eac2ab8ef3b4a93333dd79df97650fbb45bf.zip"],  # No releases, we'll just use this commit
)

# For caf-net: the http parser
http_archive(
    name = "node_http",
    build_file = "@//external:node-http.BUILD",
    strip_prefix = "http-parser-2.8.1",
    urls = ["https://github.com/nodejs/http-parser/archive/v2.8.1.tar.gz"],
)
