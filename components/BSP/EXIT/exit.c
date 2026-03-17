#include "exit.h"

//中断服务函数 
//IRAM_ATTR 宏指令 在run中运行，提高中断响应速度
void IRAM_ATTR exit_key_isr_handler(void* arg)
{
    uint32_t gpio_num= (uint32_t)arg;
    if(gpio_num==BOOT_EXIT_GPIO_NUM)
    {
        LED_TOGGLE();
    }
}

void exit_init(void)
{
    gpio_config_t gpio_init_struct={0};
    gpio_init_struct.pin_bit_mask=1ULL<<BOOT_EXIT_GPIO_NUM;
    gpio_init_struct.mode=GPIO_MODE_INPUT;
    gpio_init_struct.pull_up_en=GPIO_PULLUP_ENABLE;
    gpio_init_struct.pull_down_en=GPIO_PULLDOWN_DISABLE;
    gpio_init_struct.intr_type=GPIO_INTR_NEGEDGE;

    gpio_config(&gpio_init_struct);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(BOOT_EXIT_GPIO_NUM,exit_key_isr_handler,(void*)BOOT_EXIT_GPIO_NUM);
}   