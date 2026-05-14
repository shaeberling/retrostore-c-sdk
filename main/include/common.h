#pragma once

#include <stdlib.h>
#include <esp_heap_caps.h>

struct RsData {
  uint8_t* data;
  int len;
  bool delete_on_destruct;

  explicit RsData(bool delete_on_destruct_param = true) {
    data = NULL;
    len = 0;
    delete_on_destruct = delete_on_destruct_param;
  }

  RsData(int size) {
    // Prefer PSRAM so HTTP-response buffers don't pile up in internal SRAM
    // (where they would push Wi-Fi/BT coex working memory below the
    // threshold and cause association timeouts on ESP32-S3). Falls back
    // to any 8-bit-addressable RAM if PSRAM isn't available.
    data = (uint8_t*) heap_caps_malloc_prefer(size, 2,
                                              MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT,
                                              MALLOC_CAP_8BIT);
    len = size;
  }

  ~RsData() {
    if (delete_on_destruct && data) {
      free(data);
    }
  }
};
