#include "retrostore.h"

#include "default-data-fetcher.h"

#include <string>

// #include "rs_proto_api.pb.h"
// #include "pb_decode.h"
// #include "pb_encode.h"

namespace retrostore {

using std::string;

namespace {
// TODO: Secure, HTTPS(443). Use WiFiClientSecure.
const int DEFAULT_PORT = 80;
const string PATH_FETCH_APPS = "/api/listApps";
const string PATH_FETCH_MEDIA_IMAGES = "/api/fetchMediaImages";
}

// namespace {
// // Returns the length (in bytes) of the content in this HTTP response.
// int GetHttpResponseContentLength(uint8_t* http_response) {
//   // First we need to find the content length which we get in the header.
//   string buffer_str((const char*)http_response);
//   string content_length_key = "Content-Length: ";
//   int start_pos = buffer_str.find(content_length_key) + content_length_key.size();
//   if (start_pos < 0) {
//     Serial.println("Cannot find 'Content-Length:' marker.");
//     return -1;
//   }
//   int end_pos = buffer_str.find("\r\n", start_pos);
//   if (end_pos < 0) {
//     Serial.println("Cannot find end of content-lenth value.");
//     return -1;
//   }

//   if (start_pos >= end_pos) {
//     Serial.println("start >= end for content-length value range.");
//     return -1;
//   }
//   auto content_length_str = buffer_str.substr(start_pos, end_pos - start_pos);
//   return atoi(content_length_str.c_str());
// }
//
// }  // namespace

RetroStore::RetroStore()
    : data_fetcher_(new DefaultDataFetcher()) {
}

RetroStore::RetroStore(DataFetcher* data_fetcher)
    : data_fetcher_(data_fetcher) {
}

void RetroStore::PrintVersion() {
  // Serial.println("API Impl 2022-10-06");
}

bool RetroStore::FetchApps(int start, int num) {
  // Serial.println("FetchApps())");
  // data_fetcher_->Fetch(PATH_FETCH_APPS);
  return true;
}

void RetroStore::FetchMediaImages(const string& appId) {
  // Serial.println("FetchMediaImages()");
}

}  // namespace retrostore