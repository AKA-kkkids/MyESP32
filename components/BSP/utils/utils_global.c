#include "utils_global.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
uint32_t GetSysTickMs(void)
{
    return xTaskGetTickCount() / portTICK_PERIOD_MS;
}