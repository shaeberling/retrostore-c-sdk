#include "retrostore.h"

#include "data-fetcher-esp.h"

#include <memory>
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

static bool pb_memory_region_decode_cb(pb_istream_t* stream,
                                       const pb_field_t* field,
                                       void** arg) {
  std::unique_ptr<uint8_t> bytes(static_cast<uint8_t*>(malloc(sizeof(pb_byte_t) * stream->bytes_left)));
  pb_read(stream, (pb_byte_t*) bytes.get(), stream->bytes_left);
  RsSystemState* state = static_cast<RsSystemState*>(*arg);

  RsMemoryRegion newRegion;
  newRegion.data = std::move(bytes);
  state->regions.push_back(std::move(newRegion));

  return true;  // success
}

}

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

bool RetroStore::DownloadState(int token, RsSystemState* state) {
  // Create params object and set token.
  DownloadSystemStateParams params = DownloadSystemStateParams_init_zero;
  params.token = token;

  // Create buffer for params.
  RsData buffer(128);

  pb_ostream_t stream_param = pb_ostream_from_buffer(buffer.data, sizeof(buffer.len));
  // Encode the object to the buffer stream above.
  if (!pb_encode(&stream_param, DownloadSystemStateParams_fields, &params)) {
      ESP_LOGE(TAG, "Encoding failed: %s", PB_GET_ERROR(&stream_param));
      return false;
  }
  buffer.len = stream_param.bytes_written;
  ESP_LOGI(TAG, "DownloadSystemStateParams created. Size: %d", buffer.len);

  RsData recv_buffer;
  data_fetcher_->Fetch(PATH_DOWNLOAD_STATE, buffer, &recv_buffer);

  ESP_LOGI(TAG, "Received %d bytes response.", recv_buffer.len);

  ApiResponseDownloadSystemState stateResp = ApiResponseDownloadSystemState_init_zero;
  auto num_mem_regions = sizeof(stateResp.systemState.memoryRegions)/sizeof(stateResp.systemState.memoryRegions[0]);
  for (int i = 0; i < num_mem_regions; ++i) {
    stateResp.systemState.memoryRegions[i].data.funcs.decode = &pb_memory_region_decode_cb;
    stateResp.systemState.memoryRegions[i].data.arg = state;
  }

  pb_istream_t stream_in = pb_istream_from_buffer(recv_buffer.data, recv_buffer.len);
  if (!pb_decode(&stream_in, ApiResponseDownloadSystemState_fields, &stateResp)) {
      ESP_LOGE(TAG, "Decoding failed: %s", PB_GET_ERROR(&stream_in));
      return false;
  }
  ESP_LOGI(TAG, "DownloadSystemState response decoded successfully.");

  if (!stateResp.success) {
    ESP_LOGW(TAG, "Bad request. Server responded: %s", stateResp.message);
    return false;
  }
  if (!stateResp.has_systemState) {
    ESP_LOGE(TAG, "Server did not sent a SystemState despite reporting a success.");
    return false;
  }
  auto& _state = stateResp.systemState;

  // Copy registers
  state->registers.ix = _state.registers.ix;
  state->registers.iy = _state.registers.iy;
  state->registers.pc = _state.registers.pc;
  state->registers.sp = _state.registers.sp;
  state->registers.af = _state.registers.af;
  state->registers.bc = _state.registers.bc;
  state->registers.de = _state.registers.de;
  state->registers.hl = _state.registers.hl;
  state->registers.af_prime = _state.registers.af_prime;
  state->registers.bc_prime = _state.registers.bc_prime;
  state->registers.de_prime = _state.registers.de_prime;
  state->registers.hl_prime = _state.registers.hl_prime;
  state->registers.i = _state.registers.i;
  state->registers.r_1 = _state.registers.r_1;
  state->registers.r_2 = _state.registers.r_2;

  // Load the memoriy regions, which are dynamic and potentially large in size.
  for (int i = 0; i < _state.memoryRegions_count; ++i) {
    ESP_LOGI(TAG, "Memory region[%d], Start(%d), Length(%d)",
             i, _state.memoryRegions[i].start, _state.memoryRegions[i].length);
    state->regions[i].start = _state.memoryRegions[i].start;
    state->regions[i].length = _state.memoryRegions[i].length;
    // Note: Data has already been read/decoded.
  }
  return true;
}

}  // namespace retrostore