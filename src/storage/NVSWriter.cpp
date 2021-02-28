#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "NVSWriter.h"

NVSWriter::NVSWriter() {}

NVSWriter::~NVSWriter() {}

bool NVSWriter::init()
{
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        printf("NVM init error - erazing flash!");
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    if (err == ESP_OK)
    {
        isInitialized = true;
        return true;
    }
    return false;
}

int32_t NVSWriter::read_i32(const char *key, nvs_handle *handle)
{
    // Read
    printf("Reading from NVS ... ");
    int32_t i32value = 0; // value will default to 0, if not set yet in NVS
    esp_err_t err = nvs_get_i32(*handle, "restart_counter", &i32value);
    switch (err)
    {
    case ESP_OK:
        printf("NVM read value = %d\n", i32value);
        break;
    case ESP_ERR_NVS_NOT_FOUND:
        printf("The value is not initialized yet!\n");
        break;
    default:
        printf("Error (%s) reading!\n", esp_err_to_name(err));
    }
    return i32value;
}

uint32_t NVSWriter::updateCount(uint8_t day, uint32_t count)
{
    esp_err_t err;

    if (!isInitialized && !init())
    {
        printf("NVS init error, aborting...");
        return count;
    }

    if (!nvsCountHandle)
    {
        printf("\nOpening Non-Volatile Storage (NVS) handle... ");
        err = nvs_open(STEPCT_HANDLE, NVS_READWRITE, &nvsCountHandle);
        if (err != ESP_OK)
        {
            printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
            return count;
        }
        printf("NVS got handle");
    }

    char key[NVS_KEY_SIZE];
    sprintf(key, "%s%d", STEPCT_DAY, 0);
    err = nvs_set_i32(nvsCountHandle, key, count);
    printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

    // Commit written value.
    // After setting any values, nvs_commit() must be called to ensure changes are written
    // to flash storage. Implementations may write to storage at other times,
    // but this is not guaranteed.
    printf("Committing updates in NVS ... ");
    err = nvs_commit(nvsCountHandle);
    printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

    // Close
    nvs_close(nvsCountHandle);
    nvsCountHandle = 0;

    fflush(stdout);
    return count;
}