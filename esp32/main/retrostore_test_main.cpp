/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"

#include "retrostore.h"

static const char *TAG = "retrostore-tester";

using namespace std;
using namespace retrostore;

void testUploadDownloadSystemImage() {
  // TODO: Upload a system image for testing ...
  // TODO. Replace this hardcoded token with the one we got from the upload.
  int token = 696;

  RetroStore rs;

}


extern "C" {

void app_main(void)
{
    ESP_LOGI(TAG, "RetroStore API tests running...");

    ESP_LOGI(TAG, "[1] testUploadDownloadSystemImage...");
    testUploadDownloadSystemImage();

    ESP_LOGI(TAG, "DONE. All tests run.");

    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), WiFi%s%s, ",
            CONFIG_IDF_TARGET,
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());
}

}
