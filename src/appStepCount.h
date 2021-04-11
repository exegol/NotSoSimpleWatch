#ifndef APP_STEP_COUNT_H
#define APP_STEP_COUNT_H

#define STEPCT_DAY_ENTRY 20
#include "storage/NVSWriter.h"

/**
 * @param writer pointer to NWSWriter object for retrieving counter values
 * @param event_cb event callback to return to the calling menu
 */
void appShowStepCount(NVSWriter *writer, lv_event_cb_t event_cb);

void stepCountHide();

#endif 