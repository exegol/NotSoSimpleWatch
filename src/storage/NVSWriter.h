#ifndef NVSWRITER_H
#define NVSWRITER_H

#include "config.h"
#include <Arduino.h>
#include "gui.h"
#include "nvs_flash.h"
#include "nvs.h"


#define STEPCT_CURDAY "StepC_CurDay" //current day
#define STEPCT_STODAYS "StepC_StoDays" //no of stored days
#define STEPCT_DAY "StepC_Day_" // stored day counts _0 ... _N
#define STEPCT_HANDLE "StepCounter"
#define NVS_KEY_SIZE 16

class NVSWriter
{
public:
    
    NVSWriter();

    ~NVSWriter();

    bool init();

    void writeCount(uint8_t day, uint32_t count);

    int32_t read_i32(const char *key, nvs_handle *handle);

    uint32_t getCount(uint8_t day, uint8_t index = 0);

    uint32_t updateCount(uint8_t day, uint32_t count);

private:
    bool isInitialized = false;
    bool hasError = false;
    nvs_handle nvsCountHandle = 0;
};

#endif
