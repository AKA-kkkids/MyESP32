#ifndef _USART_H
#define _USART_H

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/uart_select.h"
#include "driver/gpio.h"


/* ���źʹ��ڶ��� */
#define USART_UX            UART_NUM_0
#define USART_TX_GPIO_PIN   GPIO_NUM_43
#define USART_RX_GPIO_PIN   GPIO_NUM_44

/* ���ڽ�����ض��� */
#define RX_BUF_SIZE         1024    /* ���λ�������С */

/* �������� */
void usart_init(uint32_t baudrate); /* ��ʼ������ */

#endif
