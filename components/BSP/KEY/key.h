#ifndef __KEY_H_
#define __KEY_H_
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define DEBOUNCE_TIME_MS 50 //按键消抖时间
#define LONG_PRESS_TIME_MS 1000 //长按时间阈值
/*
KEY_COUNT 5 按键数量 
BOOT_KEY_NUM GPIO_NUM_0 boot
KEY0_KEY_NUM GPIO_NUM_7 右
KEY1_KEY_NUM GPIO_NUM_6 下
KEY2_KEY_NUM GPIO_NUM_5 左
KEY3_KEY_NUM GPIO_NUM_4 上
*/
typedef enum {
    BOOT_KEY_NUM = 0,
    KEY0_RIGHTKEY_NUM,
    KEY1_DOWNKEY_NUM,
    KEY2_LEFTKEY_NUM,
    KEY3_UPKEY_NUM,
    KEY_COUNT_MAX
}KEY_DEV_MAX;

static const uint8_t key_pins[KEY_COUNT_MAX] = {GPIO_NUM_0};

typedef enum{
    KEY_STATUS_LOOSEN = 0,  //按键松开
    KEY_STATUS_PRESS        //按键按下
}KEY_STATUS_TYPE;

typedef enum {
    KEY_EVENT_NONE = 0,     //无事件
    KEY_EVENT_CLICK,        //单击事件
    KEY_EVENT_LONG_CLICK    //双击事件
}KEY_EVENT_TYPE;

typedef struct {
    uint8_t state   :1  ;           //按键状态:按下/松开
    uint8_t event   :2  ;           //按键事件：无/单击/双击
    uint8_t         :0  ;           //预留位
    uint8_t         :4;             //预留位
    uint64_t set_long_time  :32;     //长按时间阈值
    uint64_t press_time     :32;     //按键按下时间
}key_state;

void key_init();
int get_key_state(KEY_DEV_MAX key_num);
int get_key_event(KEY_DEV_MAX key_num);
void key_scan_task(void *pvParameters);

#endif