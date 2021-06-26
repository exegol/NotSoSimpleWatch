#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "LogWriter.h"

LogWriter LogWriter::getLogger()
{
    if (_logger == nullptr) 
    {
        _logger = new LogWriter();
    }
}