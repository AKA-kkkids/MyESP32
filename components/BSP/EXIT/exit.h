#ifndef _EXIT_H_
#define _EXIT_H_

#include "driver/gpio.h"
#include "esp_system.h"
#include "led.h"

#define BOOT_EXIT_GPIO_NUM GPIO_NUM_0
#define BOOT gpio_get_level(BOOT_EXIT_GPIO_NUM)

void exit_init(void);
#endif