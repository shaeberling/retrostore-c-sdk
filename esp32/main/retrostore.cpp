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

  ApiResponseDownloadSystemState stateResp = ApiResponseDownloadSystemState_init_zero;
  pb_istream_t stream_in = pb_istream_from_buffer(recv_buffer.data, recv_buffer.len);

  if (!pb_decode(&stream_in, ApiResponseDownloadSystemState_fields, &stateResp)) {
      ESP_LOGE(TAG, "Decoding failed: %s", PB_GET_ERROR(&stream_in));
      return;
  }
  ESP_LOGI(TAG, "DownloadSystemState response decoded successfully.");

  if (!stateResp.success) {
    ESP_LOGW(TAG, "Bad request. Server responded: %s", stateResp.message);
    return;
  }
  if (!stateResp.has_systemState) {
    ESP_LOGE(TAG, "Server did not sent a SystemState despite reporting a success.");
    return;
  }


  auto& state = stateResp.systemState;
  ESP_LOGI(TAG, "System State:==============");
  ESP_LOGI(TAG, " |> Model  : %d", state.model);
  ESP_LOGI(TAG, " |> Reg IX : %d", state.registers.ix);
  ESP_LOGI(TAG, " |> Reg IY : %d", state.registers.iy);
  ESP_LOGI(TAG, " |> Reg PC : %d", state.registers.pc);
  ESP_LOGI(TAG, " |> Reg SP : %d", state.registers.sp);
  ESP_LOGI(TAG, " |> Reg AF : %d", state.registers.af);
  ESP_LOGI(TAG, " |> Reg BC : %d", state.registers.bc);
  ESP_LOGI(TAG, " |> Reg DE : %d", state.registers.de);
  ESP_LOGI(TAG, " |> Reg HL : %d", state.registers.hl);
  ESP_LOGI(TAG, " |> Reg AFp: %d", state.registers.af_prime);
  ESP_LOGI(TAG, " |> Reg BCp: %d", state.registers.bc_prime);
  ESP_LOGI(TAG, " |> Reg DEp: %d", state.registers.de_prime);
  ESP_LOGI(TAG, " |> Reg HLp: %d", state.registers.hl_prime);
  ESP_LOGI(TAG, " |> Reg I  : %d", state.registers.i);
  ESP_LOGI(TAG, " |> Reg R1 : %d", state.registers.r_1);
  ESP_LOGI(TAG, " |> Reg R2 : %d", state.registers.r_2);
  ESP_LOGI(TAG, " |> Memory Regions [%d]:", state.memoryRegions_count);

  // Load the memoriy regions, which are dynamic and potentially large in size.
  for (int i = 0; i < state.memoryRegions_count; ++i) {
    ESP_LOGI(TAG, " |> Region[%d] start [%d]:", i, state.memoryRegions[i].start);
  }

  // TODO: memoryRegions
}

}  // namespace retrostore