#include "retrostore.h"

#include "data-fetcher-esp.h"

#include <memory>
#include <string>

#include "esp_log.h"

#include "common.h"
#include "proto/ApiProtos.pb.h"
#include "proto/pb_decode.h"
#include "proto/pb_encode.h"

#define ARRAY_SIZE(arr)     (sizeof(arr) / sizeof((arr)[0]))

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
const string PATH_GET_APP = "/api/getApp";
const string PATH_FETCH_APPS = "/api/listApps";
const string PATH_FETCH_APPS_NANO = "/api/listAppsNano";
const string PATH_FETCH_MEDIA_IMAGES = "/api/fetchMediaImages";

static RsTrs80Model fromPbModel(Trs80Model pbModel) {
  switch (pbModel) {
    case Trs80Model_MODEL_I:
      return RsTrs80Model_MODEL_I;
    case Trs80Model_MODEL_III:
      return RsTrs80Model_MODEL_III;
    case Trs80Model_MODEL_4:
      return RsTrs80Model_MODEL_4;
    case Trs80Model_MODEL_4P:
      return RsTrs80Model_MODEL_4P;
    default:
      return RsTrs80Model_UNKNOWN_MODEL;
  }
}
static Trs80Model toPbModel(RsTrs80Model model) {
  switch (model) {
    case RsTrs80Model_MODEL_I:
      return Trs80Model_MODEL_I;
    case RsTrs80Model_MODEL_III:
      return Trs80Model_MODEL_III;
    case RsTrs80Model_MODEL_4:
      return Trs80Model_MODEL_4;
    case RsTrs80Model_MODEL_4P:
      return Trs80Model_MODEL_4P;
    case RsTrs80Model_UNKNOWN_MODEL:
      return Trs80Model_UNKNOWN_MODEL;
  }
  return Trs80Model_UNKNOWN_MODEL;
}

static RsMediaType fromPbMediaType(MediaType type) {
  switch (type) {
    case MediaType_UNKNOWN:
      return RsMediaType_UNKNOWN;
    case MediaType_DISK:
      return RsMediaType_DISK;
    case MediaType_CASSETTE:
      return RsMediaType_CASSETTE;
    case MediaType_COMMAND:
      return RsMediaType_COMMAND;
    case MediaType_BASIC:
      return RsMediaType_BASIC;
  }
  return RsMediaType_UNKNOWN;
}

static MediaType toPbMediaType(RsMediaType type) {
  switch (type) {
    case RsMediaType_UNKNOWN:
      return MediaType_UNKNOWN;
    case RsMediaType_DISK:
      return MediaType_DISK;
    case RsMediaType_CASSETTE:
      return MediaType_CASSETTE;
    case RsMediaType_COMMAND:
      return MediaType_COMMAND;
    case RsMediaType_BASIC:
      return MediaType_BASIC;
  }
  return MediaType_UNKNOWN;
}

static bool pb_memory_region_decode_cb(pb_istream_t* stream,
                                       const pb_field_t* field,
                                       void** arg) {
  // FIXME: This is not called when data is zero. Don't even send it from the server!
  std::unique_ptr<uint8_t> bytes(static_cast<uint8_t*>(malloc(sizeof(pb_byte_t) * stream->bytes_left)));
  pb_read(stream, (pb_byte_t*) bytes.get(), stream->bytes_left);
  RsSystemState* state = static_cast<RsSystemState*>(*arg);

  RsMemoryRegion newRegion;
  newRegion.data = std::move(bytes);
  state->regions.push_back(std::move(newRegion));

  return true;  // success
}

static bool pb_memory_region_encode_cb(pb_ostream_t* stream,
                                       const pb_field_t* field,
                                       void * const *arg) {

  ESP_LOGI(TAG, "Encoding memory region!");
  if (!pb_encode_tag_for_field(stream, field)) {
    return false;
  }

  RsMemoryRegion* region = static_cast<RsMemoryRegion*>(*arg);
  return pb_encode_string(stream, region->data.get(), region->length);
}

static void convertApp(const App pbApp, RsApp* app) {
  app->id = pbApp.id;
  app->name = pbApp.name;
  app->version = pbApp.version;
  app->description = pbApp.description;
  ESP_LOGI(TAG, "Description length: %d", strlen(pbApp.description));
  app->release_year = pbApp.release_year;
  app->author = pbApp.author;
  app->model = fromPbModel(pbApp.ext_trs80.model);

  for (int i = 0; i < pbApp.screenshot_url_count; ++i) {
    app->screenshot_urls.push_back(std::string(pbApp.screenshot_url[i]));
  }
}

static void convertAppNano(const AppNano pbApp, RsAppNano* app) {
  app->id = pbApp.id;
  app->name = pbApp.name;
  app->version = pbApp.version;
  app->release_year = pbApp.release_year;
  app->author = pbApp.author;
  app->model = fromPbModel(pbApp.ext_trs80.model);
}

// Called from nanopb to parse App of the response.
static bool pb_App_callback(pb_istream_t* stream,
                                  const pb_field_t* field,
                                  void** arg) {
  auto* apps = static_cast<std::vector<RsApp>*>(*arg);
  App app;
  if (!pb_decode(stream, App_fields, &app)) {
    ESP_LOGE(TAG, "Failed to decode App");
    return false;
  }

  RsApp rsApp;
  convertApp(app, &rsApp);
  apps->push_back(rsApp);
  return true;
}

// Called from nanopb to parse AppNano of the response.
static bool pb_AppNano_callback(pb_istream_t* stream,
                                  const pb_field_t* field,
                                  void** arg) {
  auto* apps = static_cast<std::vector<RsAppNano>*>(*arg);
  AppNano app;
  if (!pb_decode(stream, AppNano_fields, &app)) {
    ESP_LOGE(TAG, "Failed to decode AppNano");
    return false;
  }

  RsAppNano appNano;
  convertAppNano(app, &appNano);
  apps->push_back(appNano);
  return true;
}

static bool pb_MediaImageData_callback(pb_istream_t* stream,
                                       const pb_field_t* field,
                                       void** arg) {
  auto* image = static_cast<RsMediaImage*>(*arg);
  image->data_size = stream->bytes_left;

  std::unique_ptr<uint8_t> bytes(static_cast<uint8_t*>(malloc(sizeof(pb_byte_t) * stream->bytes_left)));
  auto success = pb_read(stream, (pb_byte_t*) bytes.get(), stream->bytes_left);
  if (!success) {
    ESP_LOGE(TAG, "Error while reading media image bytes.");
    return false;
  }

  image->data = std::move(bytes);
  return true;
}

// Called from nanopb to parse AppNano of the response.
static bool pb_MediaImage_callback(pb_istream_t* stream,
                                   const pb_field_t* field,
                                   void** arg) {
  RsMediaImage rsImage;
  MediaImage image;

  image.data.arg = &rsImage;
  image.data.funcs.decode = &pb_MediaImageData_callback;

  if (!pb_decode(stream, MediaImage_fields, &image)) {
    ESP_LOGE(TAG, "Failed to decode MediaImage");
    return false;
  }

  // TODO: Decode data
  rsImage.type = fromPbMediaType(image.type);
  rsImage.uploadTime = image.uploadTime;
  rsImage.filename = image.filename;


  auto* images = static_cast<std::vector<RsMediaImage>*>(*arg);
  images->push_back(std::move(rsImage));
  return true;
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

bool RetroStore::FetchApp(const std::string& appId, RsApp* app) {
  // Create parameter object.
  GetAppParams params = GetAppParams_init_zero;
  strcpy(params.app_id, appId.c_str());

  // Create buffer for params.
  RsData buffer(64);
  pb_ostream_t stream_param = pb_ostream_from_buffer(buffer.data, buffer.len);
  // Encode the object to the buffer stream above.
  if (!pb_encode(&stream_param, GetAppParams_fields, &params)) {
      ESP_LOGE(TAG, "Encoding params failed: %s", PB_GET_ERROR(&stream_param));
      return false;
  }
  buffer.len = stream_param.bytes_written;
  ESP_LOGI(TAG, "GetAppParams created. Size: %d", buffer.len);

  RsData recv_buffer;
  bool success = data_fetcher_->Fetch(PATH_GET_APP, buffer, &recv_buffer);
  if (!success) {
    ESP_LOGE(TAG, "Error fetching data");
    return false;
  }

  ESP_LOGI(TAG, "Received %d bytes response.", recv_buffer.len);
  if (recv_buffer.len == 0) {
    return false;
  }

  // Parse the response.
  std::vector<RsApp> apps;
  ApiResponseApps resp = ApiResponseApps_init_zero;
  resp.app.arg = &apps;
  resp.app.funcs.decode = &pb_App_callback;
  pb_istream_t stream_in = pb_istream_from_buffer(recv_buffer.data, recv_buffer.len);
  if (!pb_decode(&stream_in, ApiResponseApps_fields, &resp)) {
      ESP_LOGE(TAG, "Decoding failed: %s", PB_GET_ERROR(&stream_in));
      return false;
  }
  ESP_LOGI(TAG, "ApiResponseApps decoded successfully.");

  if (!resp.success) {
    ESP_LOGW(TAG, "Bad request. Server responded: %s", resp.message);
    return false;
  }
  if (apps.size() != 1) {
    ESP_LOGE(TAG, "Exactly one app expected, but got: %d", apps.size());
    return false;
  }
  *app = apps[0];
  return true;
}

bool RetroStore::FetchApps(int start, int num, std::vector<RsApp>* apps) {
  return FetchApps(start, num, "", apps);
}

bool RetroStore::FetchApps(int start, int num, const std::string& query, std::vector<RsApp>* apps) {
  ListAppsParams params = ListAppsParams_init_zero;
  params.start = start;
  params.num = num;
  if (!query.empty()) {
    strcpy(params.query, query.c_str());
  }

  // Create buffer for params.
  RsData buffer(200);
  pb_ostream_t stream_param = pb_ostream_from_buffer(buffer.data, buffer.len);
  // Encode the object to the buffer stream above.
  if (!pb_encode(&stream_param, ListAppsParams_fields, &params)) {
      ESP_LOGE(TAG, "Encoding params failed: %s", PB_GET_ERROR(&stream_param));
      return false;
  }
  buffer.len = stream_param.bytes_written;
  ESP_LOGI(TAG, "ListAppsParams created. Size: %d", buffer.len);

  RsData recv_buffer;
  bool success = data_fetcher_->Fetch(PATH_FETCH_APPS, buffer, &recv_buffer);
  if (!success) {
    ESP_LOGE(TAG, "Error fetching data");
    return false;
  }
  ESP_LOGI(TAG, "Received %d bytes response.", recv_buffer.len);
  if (recv_buffer.len == 0) {
    return false;
  }

  // Parse the response.
  ApiResponseApps resp = ApiResponseApps_init_zero;
  resp.app.arg = apps;
  resp.app.funcs.decode = &pb_App_callback;
  pb_istream_t stream_in = pb_istream_from_buffer(recv_buffer.data, recv_buffer.len);
  if (!pb_decode(&stream_in, ApiResponseApps_fields, &resp)) {
      ESP_LOGE(TAG, "Decoding failed: %s", PB_GET_ERROR(&stream_in));
      return false;
  }
  ESP_LOGI(TAG, "ApiResponseApps decoded successfully.");

  if (!resp.success) {
    ESP_LOGW(TAG, "Bad request. Server responded: %s", resp.message);
    return false;
  }
  if (apps->size() > num) {
    ESP_LOGE(TAG, "Max %d apps expected, but got: %d", num, apps->size());
    return false;
  }
  return true;
}

bool RetroStore::FetchAppsNano(int start, int num, std::vector<RsAppNano>* apps) {
  return FetchAppsNano(start, num, "", apps);
}

bool RetroStore::FetchAppsNano(int start, int num, const std::string& query, std::vector<RsAppNano>* apps) {
  ListAppsParams params = ListAppsParams_init_zero;
  params.start = start;
  params.num = num;
  if (!query.empty()) {
    strcpy(params.query, query.c_str());
  }

  // Create buffer for params.
  RsData buffer(200);
  pb_ostream_t stream_param = pb_ostream_from_buffer(buffer.data, buffer.len);
  // Encode the object to the buffer stream above.
  if (!pb_encode(&stream_param, ListAppsParams_fields, &params)) {
      ESP_LOGE(TAG, "Encoding params failed: %s", PB_GET_ERROR(&stream_param));
      return false;
  }
  buffer.len = stream_param.bytes_written;
  ESP_LOGI(TAG, "ListAppsParams created. Size: %d", buffer.len);

  RsData recv_buffer;
  bool success = data_fetcher_->Fetch(PATH_FETCH_APPS_NANO, buffer, &recv_buffer);
  if (!success) {
    ESP_LOGE(TAG, "Error fetching data");
    return false;
  }
  ESP_LOGI(TAG, "Received %d bytes response.", recv_buffer.len);
  if (recv_buffer.len == 0) {
    return false;
  }

  // Parse the response.
  ApiResponseAppsNano resp = ApiResponseAppsNano_init_zero;
  resp.app.arg = apps;
  resp.app.funcs.decode = &pb_AppNano_callback;
  pb_istream_t stream_in = pb_istream_from_buffer(recv_buffer.data, recv_buffer.len);
  if (!pb_decode(&stream_in, ApiResponseAppsNano_fields, &resp)) {
      ESP_LOGE(TAG, "Decoding failed: %s", PB_GET_ERROR(&stream_in));
      return false;
  }
  ESP_LOGI(TAG, "ApiResponseApps decoded successfully.");

  if (!resp.success) {
    ESP_LOGW(TAG, "Bad request. Server responded: %s", resp.message);
    return false;
  }

  if (apps->size() > num) {
    ESP_LOGE(TAG, "Max %d apps expected, but got: %d", num, apps->size());
    return false;
  }

  return true;
}

bool RetroStore::FetchMediaImages(const string& appId,
                                  const std::vector<RsMediaType> types,
                                  std::vector<RsMediaImage>* images) {
  FetchMediaImagesParams params = FetchMediaImagesParams_init_zero;
  strcpy(params.app_id, appId.c_str());

  for (int i = 0; i < types.size(); ++i) {
    params.media_type[i] = toPbMediaType(types[i]);
  }
  params.media_type_count = types.size();

  RsData buffer(64);
  pb_ostream_t stream_param = pb_ostream_from_buffer(buffer.data, buffer.len);
  // Encode the object to the buffer stream above.
  if (!pb_encode(&stream_param, FetchMediaImagesParams_fields, &params)) {
      ESP_LOGE(TAG, "Encoding params failed: %s", PB_GET_ERROR(&stream_param));
      return false;
  }
  buffer.len = stream_param.bytes_written;
  ESP_LOGI(TAG, "FetchMediaImagesParams created. Size: %d", buffer.len);

  RsData recv_buffer;
  bool success = data_fetcher_->Fetch(PATH_FETCH_MEDIA_IMAGES, buffer, &recv_buffer);
  if (!success) {
    ESP_LOGE(TAG, "Error fetching data");
    return false;
  }
  ESP_LOGI(TAG, "Received %d bytes response.", recv_buffer.len);
  if (recv_buffer.len == 0) {
    return false;
  }

  ApiResponseMediaImages resp = ApiResponseMediaImages_init_zero;

  resp.mediaImage.arg = images;
  resp.mediaImage.funcs.decode = &pb_MediaImage_callback;

  pb_istream_t stream_in = pb_istream_from_buffer(recv_buffer.data, recv_buffer.len);
  if (!pb_decode(&stream_in, ApiResponseMediaImages_fields, &resp)) {
      ESP_LOGE(TAG, "Decoding failed: %s", PB_GET_ERROR(&stream_in));
      return false;
  }
  ESP_LOGI(TAG, "ApiResponseMediaImages decoded successfully.");

  if (!resp.success) {
    ESP_LOGW(TAG, "Bad request. Server responded: %s", resp.message);
    return false;
  }
  return true;
}

int RetroStore::UploadState(RsSystemState& state) {
  UploadSystemStateParams params = UploadSystemStateParams_init_zero;
  params.has_state = true;
  params.state.has_registers = true;

  params.state.model = toPbModel(state.model);
  params.state.registers.ix = state.registers.ix;
  params.state.registers.iy = state.registers.iy;
  params.state.registers.pc = state.registers.pc;
  params.state.registers.sp = state.registers.sp;
  params.state.registers.af = state.registers.af;
  params.state.registers.bc = state.registers.bc;
  params.state.registers.de = state.registers.de;
  params.state.registers.hl = state.registers.hl;
  params.state.registers.af_prime = state.registers.af_prime;
  params.state.registers.bc_prime = state.registers.bc_prime;
  params.state.registers.de_prime = state.registers.de_prime;
  params.state.registers.hl_prime = state.registers.hl_prime;
  params.state.registers.i = state.registers.i;
  params.state.registers.r_1 = state.registers.r_1;
  params.state.registers.r_2 = state.registers.r_2;

  if (state.regions.size() > ARRAY_SIZE(params.state.memoryRegions)) {
    ESP_LOGE(TAG, "Too many memory regions. Increase in PB options.");
    return -1;
  }
  params.state.memoryRegions_count = state.regions.size();
  for (int i = 0; i < state.regions.size(); ++i) {
    params.state.memoryRegions[i].start = state.regions[i].start;
    params.state.memoryRegions[i].length = state.regions[i].length;
    params.state.memoryRegions[i].data.funcs.encode = &pb_memory_region_encode_cb;
    params.state.memoryRegions[i].data.arg = &state.regions[i];
  }

  // Size of the params depends on the region size.
  int paramSize = 200;
  for (const auto& region : state.regions) {
    paramSize += region.length + 10;
  }

  // Create buffer for params.
  RsData buffer(paramSize);
  pb_ostream_t stream_param = pb_ostream_from_buffer(buffer.data, buffer.len);
  // Encode the object to the buffer stream above.
  if (!pb_encode(&stream_param, UploadSystemStateParams_fields, &params)) {
      ESP_LOGE(TAG, "Encoding failed: %s", PB_GET_ERROR(&stream_param));
      return -1;
  }
  buffer.len = stream_param.bytes_written;
  ESP_LOGI(TAG, "UploadSystemStateParams created. Size: %d", buffer.len);

  RsData recv_buffer;
  bool success = data_fetcher_->Fetch(PATH_UPLOAD_STATE, buffer, &recv_buffer);
  if (!success) {
    ESP_LOGE(TAG, "Error fetching data");
    return -1;
  }
  ESP_LOGI(TAG, "Received %d bytes response.", recv_buffer.len);
  if (recv_buffer.len == 0) {
    return -1;
  }

  ApiResponseUploadSystemState stateResp = ApiResponseUploadSystemState_init_zero;
  pb_istream_t stream_in = pb_istream_from_buffer(recv_buffer.data, recv_buffer.len);
  if (!pb_decode(&stream_in, ApiResponseUploadSystemState_fields, &stateResp)) {
      ESP_LOGE(TAG, "Decoding failed: %s", PB_GET_ERROR(&stream_in));
      return false;
  }
  ESP_LOGI(TAG, "UploadSystemState response decoded successfully.");

  if (!stateResp.success) {
    ESP_LOGW(TAG, "Bad request. Server responded: %s", stateResp.message);
    return false;
  }
  return stateResp.token;
}


bool RetroStore::DownloadState(int token, RsSystemState* state) {
  // Create params object and set token.
  DownloadSystemStateParams params = DownloadSystemStateParams_init_zero;
  params.token = token;

  // Create buffer for params.
  RsData buffer(128);

  pb_ostream_t stream_param = pb_ostream_from_buffer(buffer.data, buffer.len);
  // Encode the object to the buffer stream above.
  if (!pb_encode(&stream_param, DownloadSystemStateParams_fields, &params)) {
      ESP_LOGE(TAG, "Encoding failed: %s", PB_GET_ERROR(&stream_param));
      return false;
  }
  buffer.len = stream_param.bytes_written;
  ESP_LOGI(TAG, "DownloadSystemStateParams created. Size: %d", buffer.len);

  RsData recv_buffer;
  bool success = data_fetcher_->Fetch(PATH_DOWNLOAD_STATE, buffer, &recv_buffer);
  if (!success) {
    ESP_LOGE(TAG, "Error fetching data");
    return false;
  }
  ESP_LOGI(TAG, "Received %d bytes response.", recv_buffer.len);
  if (recv_buffer.len == 0) {
    return false;
  }


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

  state->model = fromPbModel(_state.model);

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
    // FIXME: There is one issue: If data is of size 0 then the callback will
    // not be called and we won't have enough regions in the vector and this
    // will therefore crash. Best to not send empty regions down.
  }
  return true;
}

}  // namespace retrostore