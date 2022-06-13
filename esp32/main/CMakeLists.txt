idf_component_register(SRCS "retrostore_test_main.cpp"
                            "data-fetcher-esp.cpp"
                            "retrostore.cpp"
                            "wifi.cpp"

                            "proto/pb_common.c"
                            "proto/pb_decode.c"
                            "proto/pb_encode.c"
                            "proto/ApiProtos.pb.c"
                       INCLUDE_DIRS "" "proto")
