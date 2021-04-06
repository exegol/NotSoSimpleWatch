#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "NVSWriter.h"

struct NVSException : public std::exception
{
    esp_err_t err;
    const char *what() const throw()
    {
        switch (err)
        {
        case ESP_ERR_NVS_NO_FREE_PAGES:
            return "NVS error: no free pages";
        case ESP_ERR_NVS_NEW_VERSION_FOUND:
            return "NVS error: newer version found";
        case ESP_ERR_NVS_NOT_FOUND:
            return "NVS error: not found, variable not initialized yet";
        }
        return "unknown NVS error";
    }
    NVSException(esp_err_t e) { err = e; }
};

NVSWriter::NVSWriter() {}

NVSWriter::~NVSWriter() {}

bool NVSWriter::init()
{
    if (isInitialized)
    {
        return true;
    }
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        Serial.printf("NVM init error!");
        //Serial.printf("NVM init error - erazing flash!");
        //ESP_ERROR_CHECK(nvs_flash_erase());
        //err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    if (err == ESP_OK)
    {
        isInitialized = true;
        Serial.println("NVM init ok");
        return true;
    }
    throw NVSException(err);
}

int32_t NVSWriter::read_i32(const char *key, nvs_handle *handle)
{
    int32_t i32value = 0; // value will default to 0, if not set yet in NVS
    esp_err_t err = nvs_get_i32(*handle, key, &i32value);
    switch (err)
    {
    case ESP_OK:
        Serial.printf("NVM read, key = %s, value = %d\n", key, i32value);
        break;
    case ESP_ERR_NVS_NOT_FOUND:
        Serial.printf("The value is not initialized yet!\n");
        return 0;
    default:
        Serial.printf("Error (%s) reading!\n", esp_err_to_name(err));
        throw NVSException(err);
    }
    return i32value;
}

void NVSWriter::write_i32(const char *key, nvs_handle *handle, int32_t value)
{
    esp_err_t err = nvs_set_i32(*handle, key, value);
    switch (err)
    {
    case ESP_OK:
        return;
    default:
        Serial.printf("Error (%s) writing!\n", esp_err_to_name(err));
        throw NVSException(err);
    }
}

void NVSWriter::dayChange(uint8_t day, uint32_t count)
{
    char key[STEPCT_KEY_SIZE];
    sprintf(key, "%s%d", STEPCT_DAY, 1);
    int32_t rcount1 = read_i32(key, &nvsCountHandle);
    int32_t retDiff = read_i32(STEPCT_DIFF, &nvsCountHandle);
    int32_t retStoDays = read_i32(STEPCT_STODAYS, &nvsCountHandle);
    Serial.printf("day change, day1: %d, diff: %d, sto.days: %d\n", rcount1, retDiff, retStoDays);

    retStoDays = retStoDays + 1 > 7 ? 7 : retStoDays + 1;
    for (int32_t i = retStoDays; i > 1; i--)
    {
        sprintf(key, "%s%d", STEPCT_DAY, i - 1);
        int32_t rcount = read_i32(key, &nvsCountHandle);
        Serial.printf("i: %d, rcount: %d\n", i, rcount);
        sprintf(key, "%s%d", STEPCT_DAY, i);
        write_i32(key, &nvsCountHandle, rcount);
    }
    write_i32(STEPCT_CURDAY, &nvsCountHandle, day);
    write_i32(STEPCT_STODAYS, &nvsCountHandle, retStoDays);
    cday = day;
    ccount = count;

    if (count < rcount1 + retDiff)
    {   //counter overflow, taking new value
        cdiff = 0;
        write_i32(key, &nvsCountHandle, count);
        write_i32(STEPCT_DIFF, &nvsCountHandle, 0);
        Serial.printf("overflow, count: %d, diff: 0\n", count);
    }
    else
    {
        cdiff = retDiff + rcount1;
        write_i32(STEPCT_DIFF, &nvsCountHandle, cdiff);
        sprintf(key, "%s%d", STEPCT_DAY, 1);
        write_i32(key, &nvsCountHandle, count - cdiff);
        Serial.printf("day change, day1: %d, diff: %d\n", count - cdiff, cdiff);
    }
}

uint32_t NVSWriter::updateCount(uint8_t day, uint32_t count)
{
    static int i = 0;
    if (i % 10 != 0)
    { // runs through on first run, then every 100th
        i++;
        Serial.printf("updateCount: i: %d, count in: %d, out: %d returning\n", i, count, ccount ? ccount - cdiff : count);
        return ccount ? ccount - cdiff : count - cdiff;
    }
    i = 1;
    esp_err_t err;
    try
    {
        if (!isInitialized && !init())
        {
            Serial.println("NVS init error, aborting...");
            return ccount ? ccount : count;
        }

        if (!nvsCountHandle)
        {
            Serial.println("\nOpening Non-Volatile Storage (NVS) handle... ");
            err = nvs_open(STEPCT_HANDLE, NVS_READWRITE, &nvsCountHandle);
            if (err != ESP_OK)
            {
                Serial.printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
                return ccount ? ccount : count;
            }
            Serial.println("NVS got handle");
        }

        if (cday == 0)
        {
            int32_t retday = read_i32(STEPCT_CURDAY, &nvsCountHandle);
            if (retday == 0) // init case
            {
                Serial.println("update count, init case");
                cday = day;
                ccount = count;
                cdiff = 0;                
                char key[STEPCT_KEY_SIZE];
                sprintf(key, "%s%d", STEPCT_DAY, 1);
                write_i32(STEPCT_CURDAY, &nvsCountHandle, day);
                write_i32(STEPCT_STODAYS, &nvsCountHandle, 1);
                write_i32(key, &nvsCountHandle, count);
                write_i32(STEPCT_DIFF, &nvsCountHandle, 0);
            }
            else if (retday != day) // day change - values need to be copied
            {
                dayChange(day, count);
            }
            else // retday == date - load data from flash
            {
                char key[STEPCT_KEY_SIZE];
                sprintf(key, "%s%d", STEPCT_DAY, 1);
                int32_t rcount1 = read_i32(key, &nvsCountHandle);
                int32_t retDiff = read_i32(STEPCT_DIFF, &nvsCountHandle);
                int32_t retStoDays = read_i32(STEPCT_STODAYS, &nvsCountHandle);

                if (count < rcount1 + retDiff) // counter overflow
                {
                    cday = day;
                    ccount = count;
                    cdiff = -rcount1; // adding old count to diff 
                    write_i32(STEPCT_DIFF, &nvsCountHandle, cdiff);
                    Serial.printf("updateCount, init, pers date fits, counter overflow, new diff: %d\n", cdiff);
                }
                else
                { // only loading + persisting new count
                    cdiff = retDiff;
                    ccount = count;
                    char key[STEPCT_KEY_SIZE];
                    sprintf(key, "%s%d", STEPCT_DAY, 1);
                    write_i32(key, &nvsCountHandle, count - cdiff);
                    Serial.printf("updateCount, new count: %d, diff: %d\n", count, cdiff);
                }
            }
        }
        else if (cday == day) // standard case, persist change
        {
            if (ccount > count) // counter overflow or reset
            {
                cdiff = count - ccount; // fixing diff to negative difference
                ccount = count;
                char key[STEPCT_KEY_SIZE];
                sprintf(key, "%s%d", STEPCT_DAY, 1);
                write_i32(key, &nvsCountHandle, count - cdiff);
                write_i32(STEPCT_DIFF, &nvsCountHandle, cdiff);
                Serial.printf("updateCount, overflow, count: %d, new diff: %d\n", count, cdiff);
            }
            else
            {
                char key[STEPCT_KEY_SIZE];
                sprintf(key, "%s%d", STEPCT_DAY, 1);
                write_i32(key, &nvsCountHandle, count - cdiff);
                Serial.printf("updateCount, new count: %d\n", count - cdiff);
            }
        }
        else if (cday != day) // day change - values need to be copied
        {
            dayChange(day, count);
        }

        // Commit written value.
        // After setting any values, nvs_commit() must be called to ensure changes are written
        // to flash storage. Implementations may write to storage at other times,
        // but this is not guaranteed.
        Serial.println("Committing updates in NVS ... ");
        err = nvs_commit(nvsCountHandle);
        Serial.printf((err != ESP_OK) ? "Failed!\n" : "Done\n");
    }
    catch (NVSException &ex)
    {
        Serial.println(ex.what());
    }

    // Close
    nvs_close(nvsCountHandle);
    nvsCountHandle = 0;

    //fflush(stdout);
    return ccount ? ccount - cdiff : count - cdiff;
}