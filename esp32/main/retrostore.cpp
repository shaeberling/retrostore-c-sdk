#include "retrostore.h"

#include "data-fetcher-esp.h"

#include <string>

#include "esp_log.h"

#include "common.h"
#include "proto/ApiProtos.pb.h"
#include "proto/pb_decode.h"
#include "proto/pb_encode.h"

namespace {
  static const char *TAG = "rs-api";
};  // namespace

namespace retrostore {

using std::string;

namespace {
// TODO: Secure, HTTPS(443). Use WiFiClientSecure.
const int DEFAULT_PORT = 80;
const string PATH_UPLOAD_STATE = "/api/uploadState";
const string PATH_DOWNLOAD_STATE = "/api/downloadState";
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
    : data_fetcher_(new DataFetcherEsp("retrostore.org")) {
}

RetroStore::RetroStore(DataFetcherEsp* data_fetcher)
    : data_fetcher_(data_fetcher) {
}

void RetroStore::PrintVersion() {
  ESP_LOGI(TAG, "RetroStore[ESP] SDK version 2022-11-06");
}

bool RetroStore::FetchApps(int start, int num) {
  ListAppsParams params;
  // Serial.println("FetchApps())");
  // data_fetcher_->Fetch(PATH_FETCH_APPS);
  return true;
}

void RetroStore::FetchMediaImages(const string& appId) {
  FetchMediaImagesParams params;
  // params.app_id = appId;
  // Serial.println("FetchMediaImages()");
}

void RetroStore::downloadState(int token) {
  // Create params object and set token.
  DownloadSystemStateParams params = DownloadSystemStateParams_init_zero;
  params.token = token;

  // Create buffer for params.
  RsData buffer(128);

  pb_ostream_t stream_param = pb_ostream_from_buffer(buffer.data, sizeof(buffer.len));
  // Encode the object to the buffer stream above.
  if (!pb_encode(&stream_param, DownloadSystemStateParams_fields, &params)) {
      ESP_LOGE(TAG, "Encoding failed: %s", PB_GET_ERROR(&stream_param));
      return;
  }
  buffer.len = stream_param.bytes_written;
  ESP_LOGI(TAG, "DownloadSystemStateParams created. Size: %d", buffer.len);

  RsData recv_buffer;
  data_fetcher_->Fetch(PATH_DOWNLOAD_STATE, buffer, &recv_buffer);

  ESP_LOGI(TAG, "Received %d bytes response.", recv_buffer.len);

  ApiResponseDownloadSystemState state = ApiResponseDownloadSystemState_init_zero;
  pb_istream_t stream_in = pb_istream_from_buffer(recv_buffer.data, recv_buffer.len);

  if (!pb_decode(&stream_in, ApiResponseDownloadSystemState_fields, &state)) {
      ESP_LOGE(TAG, "Decoding failed: %s", PB_GET_ERROR(&stream_in));
      return;
  }
  ESP_LOGI(TAG, "DownloadSystemState response decoded successfully.");

  if (!state.success) {
    ESP_LOGW(TAG, "Server responded: Request not successful");
    return;
  }
  if (!state.has_systemState) {
    ESP_LOGE(TAG, "Server did not sent a SystemState despite reporting a success.");
    return;
  }

  ESP_LOGI(TAG, "TODO: Decode message and SystemState.");


  // TODO: Parse the result.

}

}  // namespace retrostore