load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")


cc_library(
  name = "data-fetcher",
  srcs = [ "default-data-fetcher.cc" ],
  hdrs = [ "data-fetcher.h",
           "default-data-fetcher.h" ]
)

cc_library(
  name = "retrostore",
  srcs = [ "retrostore.cc" ],
  hdrs = [ "retrostore.h" ],
  deps = [
           ":data-fetcher",
           "//main/proto:api-protos",
           "@libcurl"
         ],
)

cc_binary(
    name = "test-cli",
    srcs = ["test-cli.cc"],
    deps = [":retrostore"],
)