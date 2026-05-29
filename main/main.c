#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
#include "key.h"
#include "exit.h"
#include "ledc.h"
#include "iic.h"
#include "xl9555.h"
#include "nvs_flash.h"
#include "usart.h"
#include "24cxx.h"
#include "lcd.h"
#include "esp_system.h"
#include "bsp_shell.h"
#include "esp_log.h"

static const char *TAG = "main";
//#define MY_PWM
//#define MY_KEY
//#define MY_XL9555
//#define MY_USART
//#define MY_EEPROM
#define MY_LCD

int system_reboot(void) 
{
    esp_restart(); 
    return 0;
}
MAIN_SHELL_EXPORT_CMD(system_reboot, "reboot", "reboot the esp32");

#ifdef MY_LCD

i2c_obj_t i2c0_master;

/**
 * @brief       程序入口
 * @param       无
 * @retval      无
 */
void app_main(void)
{
    uint8_t x = 0;
    esp_err_t ret;
    
    
    ret = nvs_flash_init();             /* 初始化NVS */

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    led_init();                         /* 初始化LED */
    i2c0_master = iic_init(I2C_NUM_0);  /* 初始化IIC0 */
    spi2_init();                        /* 初始化SPI2 */
    xl9555_init(i2c0_master);           /* IO扩展芯片初始化 */
    lcd_init();                         /* 初始化LCD */
    bsp_shell_init();                    /* 初始化shell */
    while (1)
    {
        switch (x)
        {
            case 0:
            {
                lcd_clear(WHITE);
                break;
            }
            case 1:
            {
                lcd_clear(BLACK);
                break;
            }
            case 2:
            {
                lcd_clear(BLUE);
                break;
            }
            case 3:
            {
                lcd_clear(RED);
                break;
            }
            case 4:
            {
                lcd_clear(MAGENTA);
                break;
            }
            case 5:
            {
                lcd_clear(GREEN);
                break;
            }
            case 6:
            {
                lcd_clear(CYAN);
                break;
            }
            case 7:
            {
                lcd_clear(YELLOW);
                break;
            }
            case 8:
            {
                lcd_clear(BRRED);
                break;
            }
            case 9:
            {
                lcd_clear(GRAY);
                break;
            }
            case 10:
            {
                lcd_clear(LGRAY);
                break;
            }
            case 11:
            {
                lcd_clear(BROWN);
                break;
            }
        }

        lcd_show_string(0, 40, 240, 32, 32, "ESP32", RED);
        lcd_show_string(0, 80, 240, 24, 24, "SPILCD TEST", RED);
        lcd_show_string(10, 110, 240, 16, 16, "ATOM@ALIENTEK", RED);
        lcd_show_string(10, 150, 240, 12, 12, "ATOM@ALIENTEK", RED);
        x++;

        if (x == 12)
        {
            x = 0;
        }

        LED_TOGGLE();
        vTaskDelay(500);
        ESP_LOGI(TAG,"SYSTEM RUNNING");
    }
}
#endif

#ifdef MY_EEPROM
i2c_obj_t i2c0_master;

const uint8_t g_text_buf[] = {"ESP32-S3 EEPROM"};   /* 要写入到24c02的字符串数组 */
#define TEXT_SIZE   sizeof(g_text_buf)              /* TEXT字符串长度 */
void app_main(void)
{
    uint16_t i = 0;
    uint8_t err = 0;
    uint8_t key;
    uint8_t datatemp[TEXT_SIZE];
    esp_err_t ret;
    
    ret = nvs_flash_init();             /* 初始化NVS */

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    led_init();
    i2c0_master = iic_init(I2C_NUM_0);
    xl9555_init(i2c0_master);           /* IO扩展芯片初始化 */
    at24cxx_init(i2c0_master);          /* 初始化24CXX */

    err = at24cxx_check();              /* 检测AT24C02 */
    
    if (err != 0)
    {
        while (1)                       /* 检测不到24c02 */
        {
            printf("24C02 check failed, please check!\n");
            vTaskDelay(500);
            LED_TOGGLE();               /* LED闪烁 */
        }
    }
    printf("24C02 Ready!\n");
    printf("\n");
    
    while(1)
    {
        key = xl9555_key_scan(0);
        
        switch (key)
        {
            case KEY0_PRES:
            {
                at24cxx_write(0, (uint8_t *)g_text_buf, TEXT_SIZE);
                printf("The data written is:%s\n", g_text_buf);
                break;
            }
            case KEY1_PRES:
            {
                at24cxx_read(0, datatemp, TEXT_SIZE);
                printf("The data read is:%s\n", datatemp);
                break;
            }
            default:
            {
                break;
            }
        }

        i++;

        if (i == 20)
        {
            LED_TOGGLE();               /* LED闪烁 */
            i = 0;
        }
        
        vTaskDelay(10);
    }   
}



#endif

#ifdef MY_USART
void app_main(void)
{
    esp_err_t ret;
    uint8_t len = 0;
    uint16_t times = 0;
    unsigned char data[RX_BUF_SIZE] = {0};

    ret = nvs_flash_init();                                                             /* 初始化NVS */

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    
    led_init();                                                                         /* 初始化LED */
    usart_init(115200);                                                                 /* 初始化串口 */

    while(1)
    {
        uart_get_buffered_data_len(USART_UX, (size_t*) &len);                           /* 获取环形缓冲区数据长度 */

        if (len > 0)                                                                    /* 判断数据长度 */
        {
            memset(data, 0, RX_BUF_SIZE);                                               /* 对缓冲区清零 */
            printf("\n您发送的消息为:\n");
            uart_read_bytes(USART_UX, data, len, 100);                                  /* 读数据 */
            uart_write_bytes(USART_UX, (const char*)data, strlen((const char*)data));   /* 写数据 */
        }
        else
        {
            times++;

            if (times % 5000 == 0)
            {
                printf("\n正点原子 ATK-DNESP32-S3 开发板 串口实验\n");
                printf("正点原子@ALIENTEK\n\n\n");
            }

            if (times % 200 == 0)
            {
                printf("请输入数据，以回车键结束\n");
            }

            if (times % 30 == 0)
            {
                LED_TOGGLE();
            }

            vTaskDelay(10);
        }
    }
}

#endif

#ifdef MY_PWM
void app_main(void) 
{
    uint8_t dir = 1; //占空比方向：递增还是递减
    uint16_t ledcpwmval = 0; //占空比值 范围：0-100

    ledc_config_t *ledc_config = malloc(sizeof(ledc_config_t));

    ledc_config->clk_cfg = LEDC_AUTO_CLK; //根据设置分辨率、占空比，自动选择LEDC的时钟源
    ledc_config->timer_num = LEDC_PWM_TIMER;
    ledc_config->freq_hz = 1000; //设置PWM频率
    ledc_config->duty_resolution = LEDC_TIMER_14_BIT; //设置占空比分辨率
    ledc_config->channel = LEDC_PWM_CH0_CHANNEL;
    ledc_config->gpio_num = LEDC_PWM_CH0_GPIO;
    ledc_config->duty = 0; //设置初始占空比
    ledc_init(ledc_config);

    while(1)
    {
        
        if(dir == 1)
        {
            ledcpwmval += 5;
        }
        else
        {
            ledcpwmval -= 5;
        }
        if(ledcpwmval == 100)
        {
            dir = 0;
        }else if(ledcpwmval == 0)
        {
            dir = 1;
        }
        ledc_pwm_set_duty(ledc_config, ledcpwmval);
        vTaskDelay(50);
    }
}
#endif

#ifdef MY_KEY
static int event = KEY_EVENT_NONE;
void app_main(void) 
{
    led_init();
    key_init();
    exit_init();
 
    while(1)
    {
        for(int i=0 ; i< KEY_COUNT_MAX ; i++)
        {
            event = get_key_event((KEY_DEV_MAX)i);
            printf("key %d event %d\n", i, event);
            if(event == KEY_EVENT_NONE)
            {
                continue;
            }

            if(event == KEY_EVENT_CLICK)
            {
                LED_TOGGLE();
            }else if(event == KEY_EVENT_LONG_CLICK)
            {
                LED(1);  
            }
        }
        vTaskDelay(50);
    }
}
#endif

#ifdef MY_XL9555
i2c_obj_t i2c0_master;

void app_main(void) 
{
    uint8_t key;
    esp_err_t ret;

    ret = nvs_flash_init();             /* 初始化NVS */
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    led_init();
    i2c0_master = iic_init(I2C_NUM_0);
    xl9555_init(i2c0_master);
    
    while(1)
    {
        key = xl9555_key_scan(0);
        
        switch (key)
        {
            case KEY0_PRES:
            {
                printf("KEY0 has been pressed \n");
                xl9555_pin_write(BEEP_IO, 0);
                break;
            }
            case KEY1_PRES:
            {
                printf("KEY1 has been pressed \n");
                xl9555_pin_write(BEEP_IO, 1);
                break;
            }
            case KEY2_PRES:
            {
                printf("KEY2 has been pressed \n");
                LED(0);
                break;
            }
            case KEY3_PRES:
            {
                printf("KEY3 has been pressed \n");
                LED(1);
                break;
            }
            default:
            {
                break;
            }
        }

        if (XL9555_INT == 0)
        {
            printf("123");
        }
        vTaskDelay(200);
    }
    
}
#endif
