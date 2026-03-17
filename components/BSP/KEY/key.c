#include "key.h"
#include "led.h"
#include "esp_log.h"
#include "utils_global.h"

#define TAG "KEY_SCAN"

static key_state key_states[KEY_COUNT_MAX] = {0};

void key_init()
{
    gpio_config_t gpio_key_init_struct={    
        .pin_bit_mask = 0,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    for(int i = 0; i < KEY_COUNT_MAX; i++)
    {
        gpio_key_init_struct.pin_bit_mask = (1ULL << key_pins[i]);
        gpio_config(&gpio_key_init_struct);
    }
    ESP_LOGI(TAG, "Key scan initialized for %d keys", KEY_COUNT_MAX);
    // 创建扫描任务 (栈大小 2048, 优先级 5)
    //xTaskCreate(key_scan_task, "key_scan_task", 2048, NULL, 5, NULL);
}

//获取按键状态
int get_key_state(KEY_DEV_MAX key_num)
{
    key_num = MIN(key_num, (KEY_DEV_MAX)(KEY_COUNT_MAX-1));
    return key_states[key_num].state;
}

//获取按键事件
int get_key_event(KEY_DEV_MAX key_num)
{
    key_num = MIN(key_num, (KEY_DEV_MAX)(KEY_COUNT_MAX-1));
    int event = key_states[key_num].event;
    if(event != KEY_EVENT_NONE)
    {
        key_states[key_num].event = KEY_EVENT_NONE;
    }
    return event;
}

void key_scan_task(void *pvParameters)
{
    while(1)
    {

        for (int i = 0; i < KEY_COUNT_MAX; i++)
        {
            if(gpio_get_level(key_pins[i]) == 0)
            {
                if(key_states[i].press_time == 0)
                {
                    key_states[i].press_time = GetSysTickMs();
                }else if((GetSysTickMs() - key_states[i].press_time) > DEBOUNCE_TIME_MS)
                {
                    key_states[i].state = KEY_STATUS_PRESS;
                    if((GetSysTickMs() - key_states[i].press_time) > LONG_PRESS_TIME_MS)
                    {
                        if((GetSysTickMs() - key_states[i].set_long_time) > LONG_PRESS_TIME_MS)
                        {
                            key_states[i].event = KEY_EVENT_LONG_CLICK;
                            key_states[i].set_long_time = GetSysTickMs();
                        }
                    }
                }
            }else
            {
                if(key_states[i].state == KEY_STATUS_PRESS)
                {
                    if((GetSysTickMs() - key_states[i].press_time) < LONG_PRESS_TIME_MS)
                    {
                        key_states[i].event = KEY_EVENT_CLICK;
                    }
                }
                key_states[i].state = KEY_STATUS_LOOSEN;
                key_states[i].press_time = 0;
            }
        }
        vTaskDelay(10);
    }
}

