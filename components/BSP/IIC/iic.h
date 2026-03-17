#ifndef __IIC_H__
#define __IIC_H__

#include "driver/i2c.h"
#include "driver/gpio.h"
#include "esp_err.h"

/*IIC控制块*/
typedef struct _ic2_obj_t{
    i2c_port_t port; // 使用的是哪个硬件IIC端口
    gpio_num_t scl; // SCL引脚号
    gpio_num_t sda; // SDA引脚号
    esp_err_t init_flag; // 初始化标志位
}i2c_obj_t;

/*读写数据结构体*/
typedef struct _ic2_buf_t{
    size_t len; // 读写长度
    uint8_t *buf; // 读写数据
}i2c_buf_t;

extern i2c_obj_t iic_master[I2C_NUM_MAX];

/*读写标志位*/
#define I2C_FLAG_READ   (0x01)
#define I2C_FLAG_STOP   (0x02)
#define I2C_FLAG_WRITE  (0x04)

/*引脚与相关参数定义*/
#define IIC0_SDA_GPIO_PIN                  GPIO_NUM_41
#define IIC0_SCL_GPIO_PIN                  GPIO_NUM_42
#define IIC1_SDA_GPIO_PIN                  GPIO_NUM_5
#define IIC1_SCL_GPIO_PIN                  GPIO_NUM_4
#define IIC_FREQ                            400000       // 400KHz 快速模式
#define I2C_MASTER_TX_BUF_DISABLE           0
#define I2C_MASTER_RX_BUF_DISABLE           0
#define ACK_CHECK_EN                        0x1

i2c_obj_t iic_init(uint8_t iic_port);
esp_err_t i2c_transfer(i2c_obj_t *self, uint16_t addr, size_t n, i2c_buf_t *bufs, unsigned int flags);

#endif