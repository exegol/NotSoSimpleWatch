
#ifndef APP_SET_TIME_H
#define APP_SET_TIME_H


#include <soc/rtc.h>
#include <Arduino.h>
#include <time.h>

void appSetTime();

void prtTime(byte digit);

int getTnum();

#endif