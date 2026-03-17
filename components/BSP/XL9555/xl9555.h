#ifndef __XL9555_H__
#define __XL9555_H__

#include "esp_err.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "iic.h"

/* 引脚与相关参数定义 */
#define XL9555_INT_IO                   GPIO_NUM_40 /* XL9555_INT 引脚 */
#define XL9555_INT                      gpio_get_level(XL9555_INT_IO) /* 读取 XL9555_INT 的电平 */
#define XL9555_ADDR                     0x20 /* 7 位器件地址 */

/* 器件寄存器 */
#define XL9555_INPUT_PORT0_REG          0 /* 输入 P0 寄存器用于读取 P0 端口的输入值 */
#define XL9555_INPUT_PORT1_REG          1 /* 输入 P1 寄存器用于读取 P1 端口的输入值 */
#define XL9555_OUTPUT_PORT0_REG         2 /* 输出 P0 寄存器用于设置 P0 端口的输出值 */
#define XL9555_OUTPUT_PORT1_REG         3 /* 输出 P1 寄存器用于设置 P1 端口的输出值 */

#define XL9555_INVERSION_PORT0_REG      4
/* 极性反转 P0 寄存器用于当 P0 端口做为输入时，对输入的电平进行反转处理，即管脚为高电平时，设置这
个寄存器中相应的位为 1 时，读取到的输入寄存器0，1 的值就是低电平0 */
#define XL9555_INVERSION_PORT1_REG      5 
/* 极性反转 P1 寄存器用于当 P1 端口做为输入时，对输入的电平进行反转处理，即管脚为高电平时，设置这
个寄存器中相应的位为 1 时，读取到的输入寄存器0，1 的值就是低电平0 */

#define XL9555_CONFIG_PORT0_REG         6 
/* 配置 P0 寄存器用于配置 P0 端口的做为输入(1)或是输出(0) */
#define XL9555_CONFIG_PORT1_REG         7 
/* 配置 P1 寄存器用于配置 P1 端口的做为输入(1)或是输出(0) */

/* XL9555 各个 IO 的功能 */
#define AP_INT_IO                      0x0001 /* AP3216C 中断引脚 P00 */
#define QMA_INT_IO                     0x0002 /* QMA6100P 中断引脚 P01 */
#define SPK_EN_IO                      0x0004 /* 功放使能引脚 P02 */
#define BEEP_IO                        0x0008 /* 蜂鸣器控制引脚 P03 */
#define OV_PWDN_IO                     0x0010 /* 摄像头待机引脚 P04 */
#define OV_RESET_IO                    0x0020 /* 摄像头复位引脚 P05 */
#define GBC_LED_IO                     0x0040 /* ATK_MODULE 接口 LED 引脚 P06 */
#define GBC_KEY_IO                     0x0080 /* ATK_MODULE 接口 KEY 引脚 P07 */
#define LCD_BL_IO                      0x0100 /* RGB 屏背光控制引脚 P10 */
#define CT_RST_IO                      0x0200 /* 触摸屏中断引脚 P11 */
#define SLCD_RST_IO                    0x0400 /* SPI_LCD 复位引脚 P12 */
#define SLCD_PWR_IO                    0x0800 /* SPI_LCD 控制背光引脚 P13 */

#define KEY3_IO                        0x1000 /* 按键 3 引脚 P14 */
#define KEY2_IO                        0x2000 /* 按键 2 引脚 P15 */
#define KEY1_IO                        0x4000 /* 按键 1 引脚 P16 */
#define KEY0_IO                        0x8000 /* 按键 0 引脚 P17 */

#define KEY0                           xl9555_pin_read(KEY0_IO) /* 读取 KEY0 引脚 */
#define KEY1                           xl9555_pin_read(KEY1_IO) /* 读取 KEY1 引脚 */
#define KEY2                           xl9555_pin_read(KEY2_IO) /* 读取 KEY2 引脚 */
#define KEY3                           xl9555_pin_read(KEY3_IO) /* 读取 KEY3 引脚 */

#define KEY0_PRES                      1 /* KEY0 按下 */
#define KEY1_PRES                      2 /* KEY1 按下 */
#define KEY2_PRES                      3 /* KEY1 按下 */
#define KEY3_PRES                      4 /* KEY1 按下 */

void xl9555_init(i2c_obj_t self);                                   /* 初始化XL9555 */
int xl9555_pin_read(uint16_t pin);                                  /* 获取某个IO状态 */
uint16_t xl9555_pin_write(uint16_t pin, int val);                   /* 控制某个IO的电平 */
esp_err_t xl9555_read_byte(uint8_t* data, size_t len);              /* 读取XL9555的16位IO值 */
uint8_t xl9555_key_scan(uint8_t mode);                              /* 扫描按键值 */
uint16_t xl9555_ioconfig(uint16_t config_value);                    /* 配置XL9555的IO */

#endif