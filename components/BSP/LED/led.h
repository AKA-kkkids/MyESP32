#ifndef __LED_H_  
#define __LED_H_  
#include "driver/gpio.h"

#define LED_GPIO_NUM    GPIO_NUM_1  

enum LED_STATE {
    LED_ON = 0,
    LED_OFF
};

#define LED(X) do{ X?\
                 gpio_set_level(LED_GPIO_NUM, LED_OFF):\
                 gpio_set_level(LED_GPIO_NUM, LED_ON);\
                 }while (0)
                 
#define LED_TOGGLE() do{gpio_set_level(LED_GPIO_NUM, !gpio_get_level(LED_GPIO_NUM));} while (0)
void led_init(void);
#endif