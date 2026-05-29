#include "freertos/FreeRTOS.h" 
#include "freertos/task.h"
#include "hal/usb_serial_jtag_ll.h"
#include "driver/uart.h"
#include "driver/usb_serial_jtag.h"
#include "bsp_shell.h"
//#include "bsp_socket.h"


Shell shell;
char shellBuffer[SHELL_PARSE_MAX_LEN];
static char is_usb_conn = false;
//static vprintf_like_t base_like = NULL;

/**
 * @brief 用户shell写
 * 
 * @param data 数据
 * @param len 数据长度
 * 
 * @return unsigned short 写入实际长度
 */
signed short userShellWrite(char *data, unsigned short len)
{
    if(!is_usb_conn)
    {
        return 0;
    }
    int send_len = 0;
    while (len!=0)
    {
        int size = usb_serial_jtag_ll_write_txfifo((const uint8_t *)&data[send_len], len<=64?len:64); 
        usb_serial_jtag_ll_txfifo_flush(); 
        send_len+=size;
        len -= size;
    }
    return send_len; 

}   


/**
 * @brief 用户shell读
 * 
 * @param data 数据
 * @param len 数据长度
 * 
 * @return unsigned short 读取实际长度
 */
signed short userShellRead(char *data, unsigned short len)
{
    size_t  rx_size = 0;
    if(usb_serial_jtag_ll_rxfifo_data_available())
    {
        rx_size = usb_serial_jtag_ll_read_rxfifo(( uint8_t *) data, len);
        is_usb_conn = true;
        return rx_size;
    }
    return 0; 
}
/*
int new_vprintf_like(const char * fmt, va_list vp)
{
    if(bsp_socket_is_connect())
    {
        char *pData = NULL; 
        int size = vasprintf(&pData, fmt, vp);
        bsp_socket_write(pData,size);
        free(pData);
    }
    return vprintf(fmt,vp);
}
*/
void  bsp_shell_init()
{
    shell.read = userShellRead;
    shell.write = userShellWrite;
    shellInit(&shell, shellBuffer, sizeof(shellBuffer));
    xTaskCreate(shellTask, "shellTask", 2048, &shell, 1, NULL);
    //base_like = esp_log_set_vprintf(new_vprintf_like);
}