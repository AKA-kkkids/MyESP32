#include "ledc.h"

//占空比-百分比转化为计数值 m-固定为2 n-为分辨率
uint32_t ledc_duty_pow(uint32_t duty, uint8_t m, uint8_t n)
{
    uint32_t result = 1;
    while(n--)
    {
        result *= m;
    }
    return (result * duty)/100;
}

void ledc_init(ledc_config_t *ledc_config)
{
    ledc_config->duty = ledc_duty_pow(ledc_config->duty, 2, ledc_config->duty_resolution);

    //ledc定时器配置：速度、分辨率、时钟、频率、定时器编号
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = ledc_config->duty_resolution,  //设置分辨率
        .clk_cfg = ledc_config->clk_cfg,
        .freq_hz = ledc_config->freq_hz,
        .timer_num = ledc_config->timer_num,
    };
    ledc_timer_config(&ledc_timer);

    //ledc通道配置：速度、中断类型、通道号、占空比、GPIO编号、hpoint偏移-直接就是高电平、定时器编号
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .intr_type = LEDC_INTR_DISABLE,
        .channel = ledc_config->channel,
        .duty = ledc_config->duty,
        .gpio_num = ledc_config->gpio_num,
        .hpoint = 0,
        .timer_sel = ledc_config->timer_num,
    };
    ledc_channel_config(&ledc_channel);
}

void ledc_pwm_set_duty(ledc_config_t *ledc_config, uint32_t duty)
{
    ledc_config->duty = ledc_duty_pow(duty, 2, ledc_config->duty_resolution);
    ledc_set_duty(LEDC_LOW_SPEED_MODE,ledc_config->channel, ledc_config->duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE,ledc_config->channel);
}