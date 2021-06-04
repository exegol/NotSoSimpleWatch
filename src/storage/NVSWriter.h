#ifndef NVSWRITER_H
#define NVSWRITER_H

#include "config.h"
#include <Arduino.h>
#include "gui.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "time.h"


#define STEPCT_CURDAY "StepC_CurDay" //current day
#define STEPCT_STODAYS "StepC_StoDays" //no of stored days
#define STEPCT_DAY "StepC_Day_" // stored day counts _0 ... _N
#define STEPCT_DIFF "StepC_Diff" //difference to counter value
#define STEPCT_HANDLE "StepCounter"
#define STEPCT_KEY_SIZE 16 //buffer size for STEPCT_DAY string
#define STEPCT_HISTORY_SIZE 7

class NVSWriter
{
public:
    
    NVSWriter();

    ~NVSWriter();

    bool init();

    void writeCount(uint8_t day, uint32_t count);

    uint32_t getCount(uint8_t day, uint8_t index = 0);

    uint32_t updateCount(uint32_t count);

    /**
     * @param day persisted day count to be returned 0 (current day) .. STEPCT_HISTORY_SIZE-1 (oldest persisted day)
     * @return number of counted steps
     */
    uint32_t getDayCount(uint8_t day);

    uint8_t getCDay();

private:

    int32_t read_i32(const char *key, nvs_handle *handle);

    void write_i32(const char *key, nvs_handle *handle, int32_t value);

    /**
     * @brief copies the day counts one position ahead, storing max. 7 days
     * loosing the oldest value.
     * Also handling counter resets/overflows - the counter seems to overflow at 65k.
     */
    void dayChange(uint8_t day, uint32_t count);

    time_t getUnixTime();

    tm* getTimeInfo();

    nvs_handle getNvsCountHandle();

    bool isInitialized = false;
    bool hasError = false;
    bool historyLoaded = false;
    nvs_handle nvsCountHandle = 0;

    //cached values
    uint8_t cday = 0;
    int32_t cdiff = 0;    
    uint32_t ccount = 0;
    uint32_t countHistory[STEPCT_HISTORY_SIZE];

};

#endif
