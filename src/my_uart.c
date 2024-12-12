/**
 * @File Name: my_uart.c
 * @brief  
 * @Author : chengyuliang email:chengyuliang@jimiiot.com
 * @Version : 1.0
 * @Update : 2024-12-12
 **
 */

#include <stdio.h>
#include <string.h>
#include "stdlib.h"

/* My header */
#include "my_uart.h"
#include "my_log.h"
#include "my_os_ctrl.h"
#include "my_gpio.h"
#include "my_type.h"

#define MY_UART_ENABLE 0 //是否启用该模块
#if MY_UART_ENABLE
#ifdef _LIOT_UART_H_
    #define LIOT_UART_RX_BUFF_SIZE 2048
    #define LIOT_UART_TX_BUFF_SIZE 2048

    extern const liot_uart_func_s liot_uart_pin_func[];
    __attribute__((used)) static bool config_uart_bit_func(int uartPort)
    {
        int uart_tx_bit     = 0;
        int uart_tx_func    = 0;
        int uart_rx_bit     = 0;
        int uart_rx_func    = 0;

        if(liot_uart_pin_func[uartPort].port == LIOT_PORT_NONE)
            return false;

        uart_tx_bit = liot_uart_pin_func[uartPort].tx_pin;
        uart_tx_func = liot_uart_pin_func[uartPort].tx_func;
        uart_rx_bit = liot_uart_pin_func[uartPort].rx_pin;
        uart_rx_func = liot_uart_pin_func[uartPort].rx_func;

        if(uart_tx_bit == LIOT_UART_PIN_NONE || uart_tx_func != LIOT_UART_FUNC_NONE)
            return false;

        liot_pin_set_func(uart_tx_bit, uart_tx_func);
        liot_pin_set_func(uart_rx_bit, uart_rx_func);

        return true;
    }

    void liot_uart_notify_cb(uint32 ind_type, liot_uart_port_number_e port, uint32 size)
    {
        unsigned char recv_buff[LIOT_UART_RX_BUFF_SIZE] = {0};
        unsigned int real_size                          = 0;
        int read_len                                    = 0;

        liot_trace("UART port %d receive ind type:0x%x, receive data size:%d", port, ind_type, size);
        switch (ind_type)
        {
            case LIOT_UART_RX_OVERFLOW_IND: // rx buffer overflow
            case LIOT_UART_RX_RECV_DATA_IND:
            {
                while (size > 0)
                {
                    real_size = MIN(size, LIOT_UART_RX_BUFF_SIZE);
                    read_len  = liot_uart_read(port, (unsigned char *)&recv_buff, real_size);
                    liot_trace("read_len=%d, recv_data=%s", read_len, recv_buff);
                    if ((read_len > 0) && (size >= read_len))
                    {
                        size -= read_len;
                    }
                    else
                    {
                        break;
                    }
                }
                break;
            }
            case LIOT_UART_TX_FIFO_COMPLETE_IND:
            {
                liot_trace("tx fifo complete");
                break;
            }
        }
    }
#endif // Lierda

void my_uart_task_thread (void *argument)
{
#ifdef _LIOT_UART_H_
    liot_task_t cur_taskRef;
    LiotOSStatus_t get_current_ref_err = liot_rtos_task_get_current_ref(&cur_taskRef);
    if (get_current_ref_err != 0)
    {
        liot_trace("get_current_ref_err:%d", get_current_ref_err);
    }

    int ret                         = 0;
    liot_uart_config_s usart_config = {0};
    usart_config.baudrate           = LIOT_UART_BAUD_115200;
    usart_config.data_bit           = LIOT_UART_DATABIT_8;
    usart_config.flow_ctrl          = LIOT_FC_NONE;
    usart_config.stop_bit           = LIOT_UART_STOP_1;
    usart_config.parity_bit         = LIOT_UART_PARITY_NONE;
    usart_config.isPrintfPort       = TRUE;


    liot_trace("my_uart_task_thread start");
    liot_rtos_task_sleep_ms(10000);
    liot_trace("==========Uart1 Init: Baudrate-%d ==========\r\n", usart_config.baudrate);

    config_uart_bit_func(LIOT_UART_PORT_1);
    liot_uart_set_dcbconfig(LIOT_UART_PORT_1, &usart_config);
    ret = liot_uart_open(LIOT_UART_PORT_1);
    
    if (ret == LIOT_UART_SUCCESS)
    {
        liot_uart_register_cb(LIOT_UART_PORT_1, liot_uart_notify_cb);
    }
    else
    {
        liot_trace("uart1_open error !!!");
    }

    liot_uart_tx_way_config(LIOT_UART_TX_DRIVER_DMA);

    while(1)
    {
        liot_trace("liot_uart_open:%d", ret);
        liot_trace("UART1_task_stack_space = %d", liot_rtos_task_get_stack_space(cur_taskRef));
        liot_uart_write(LIOT_UART_PORT_1, (unsigned char *)"AT\n", 2);
        liot_rtos_task_sleep_ms(1000);
    }

#endif // Lierda
}
#endif