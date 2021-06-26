#ifndef LOG_WRITER_H
#define LOG_WRITER_H

#include "config.h"
#include <Arduino.h>
#include "gui.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "time.h"


class LogWriter
{
public:
    
    
    static LogWriter getLogger();

   

    uint8_t getCDay();

private:

    LogWriter();

    ~LogWriter();

    static LogWriter* _logger;


};

#endif
