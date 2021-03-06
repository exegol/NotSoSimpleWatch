#ifndef APP_STEP_COUNT_H
#define APP_STEP_COUNT_H

#define STEPCT_ENTRY_STRLEN 20
#include "storage/StepWriter.h"

/**
 * @param writer pointer to NWSWriter object for retrieving counter values
 * @param event_cb event callback to return to the calling menu
 */
void appShowStepCount(StepWriter *writer, lv_event_cb_t event_cb);

void stepCountHide();

#endif 