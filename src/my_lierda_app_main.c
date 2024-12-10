/**
 * @File Name: my_lierda_app_main.c
 * @brief  
 * @Author : chengyuliang email:chengyuliang@jimiiot.com
 * @Version : 1.0
 * @Update : 2024-12-10
 **
 */

#include <stdio.h>
#include <string.h>
#include "stdlib.h"

/* My header */
#include "my_mem_ctrl.h"
#include "my_log.h"
#include "my_os_ctrl.h"
#include "my_dev.h"
#include "my_uart.h"



/* extern */
extern void my_uart_task_thread(void *argv);

/* macro definition */
#define MINI_BUFFER_SIZE 8
#define SMALL_BUFFER_SIZE 16
#define NORMAL_BUFFER_SIZE 32
#define LARGE_BUFFER_SIZE 64

#define DEVICE_INFO_GET 1
#define HEAP_INFO_GET 1
#define UART1_INFO_GET 1

#define UART1_OPEN 1

#if UART1_OPEN
    #define UART1_TASK_STACK_SIZE 4096

    static liot_StaticTask_t uart1_task_mem;
    static liot_task_t g_uart1_taskRef = NULL;

    __ALIGNED(8) static uint8_t uart1_task_stack[UART1_TASK_STACK_SIZE];

    void create_uart1_task(void)
    {
        LiotOSStatus_t result = liot_rtos_task_create_static(
                                    &g_uart1_taskRef,
                                    UART1_TASK_STACK_SIZE,
                                    APP_PRIORITY_NORMAL,
                                    "My_Uart1_Task",
                                    my_uart_task_thread,
                                    uart1_task_stack,
                                    &uart1_task_mem,
                                    NULL);
        if(result == 0)
        {
            liot_trace("uart1 task create success %d, time:%d", result, liot_rtos_get_running_time());
        }
        else
        {
            liot_trace("uart1 task create fail %d, time:%d", result, liot_rtos_get_running_time());
        }          
    }
#endif // UART1_OPEN

void liot_custom_demo_thread (void *argument)
{
    #if DEVICE_INFO_GET
        liot_errcode_dev_e ret_get_imei, ret_get_firmware_version, ret_get_sn, ret_get_product_id, ret_get_firmware_subversion, ret_get_model, ret_get_modem_fun;
        char *imei_buffer = my_create_buffer(NORMAL_BUFFER_SIZE);
        char *firmware_version_buffer = my_create_buffer(NORMAL_BUFFER_SIZE);
        char *sn_buffer = my_create_buffer(NORMAL_BUFFER_SIZE);
        char *product_id_buffer = my_create_buffer(NORMAL_BUFFER_SIZE);
        char *firmware_subversion_buffer = my_create_buffer(NORMAL_BUFFER_SIZE);
        char *model_buffer = my_create_buffer(NORMAL_BUFFER_SIZE);
        uint8_t modem_fun;

        ret_get_imei = liot_dev_get_imei(imei_buffer, NORMAL_BUFFER_SIZE, 0);
        if (ret_get_imei != 0) 
        {
            liot_trace("liot_dev_get_imei Error Code:%d", ret_get_imei);
        }
        ret_get_firmware_version = liot_dev_get_firmware_version(firmware_version_buffer, NORMAL_BUFFER_SIZE);
        if (ret_get_firmware_version != 0) 
        {
            liot_trace("liot_dev_get_firmware_version Error Code:%d", ret_get_firmware_version);
        }
        ret_get_sn = liot_dev_get_sn(sn_buffer, NORMAL_BUFFER_SIZE, 0);
        if (ret_get_sn != 0) 
        {
            liot_trace("liot_dev_get_sn Error Code:%d", ret_get_sn);
        }
        ret_get_product_id = liot_dev_get_product_id(product_id_buffer, NORMAL_BUFFER_SIZE);
        if (ret_get_product_id != 0) 
        {
            liot_trace("liot_dev_get_product_id Error Code:%d", ret_get_product_id);
        }
        ret_get_firmware_subversion = liot_dev_get_firmware_subversion(firmware_subversion_buffer, NORMAL_BUFFER_SIZE);
        if (ret_get_firmware_subversion != 0) 
        {
            liot_trace("liot_dev_get_firmware_subversion Error Code:%d", ret_get_firmware_subversion);
        }
        ret_get_model = liot_dev_get_model(model_buffer, NORMAL_BUFFER_SIZE);
        if (ret_get_model != 0) 
        {
            liot_trace("liot_dev_get_model Error Code:%d", ret_get_model);
        }
        ret_get_modem_fun = liot_dev_get_modem_fun(&modem_fun, 0);
        if (ret_get_modem_fun != 0) 
        {
            liot_trace("liot_dev_memory_size_query Error Code:%d", ret_get_modem_fun);
        }
    #endif // DEVICE_INFO_GET

    #if HEAP_INFO_GET
        liot_errcode_dev_e ret_memory_size_query;
        liot_memory_heap_state_s heap_state;

        ret_memory_size_query = liot_dev_memory_size_query(&heap_state);
        if (ret_memory_size_query != 0) 
        {
            liot_trace("liot_dev_memory_size_query Error Code:0x%X", ret_memory_size_query);
        }
    #endif // HEAP_INFO_GET

    #if UART1_OPEN
        create_uart1_task();
    #endif // UART1_OPEN

    while(1)
    {   
        liot_rtos_task_sleep_ms(5000);
        #if DEVICE_INFO_GET
            liot_trace("------------Device INFO------------");
            liot_trace("IMEI(SIM0):%s  |ret=%d", imei_buffer, ret_get_imei);
            liot_trace("firmware_version:%s  |ret=%d", firmware_version_buffer, ret_get_firmware_version);
            liot_trace("SN(SIM0):%s  |ret=%d", sn_buffer, ret_get_sn);
            liot_trace("product_id:%s  |ret=%d", product_id_buffer, ret_get_product_id);
            liot_trace("firmware_subversion:%s  |ret=%d", firmware_subversion_buffer, ret_get_firmware_subversion);
            liot_trace("model:%s  |ret=%d", model_buffer, ret_get_model);
            liot_trace("modem_fun(SIM0):%d  |ret=%d", modem_fun, ret_get_modem_fun);
        #endif //DEVICE_INFO_GET

        #if HEAP_INFO_GET
            liot_trace("------------Heap INFO------------");
            liot_trace("total_heapsize:%d and avail_heapsize:%d  |ret=0x%X", heap_state.total_size, heap_state.avail_size, ret_memory_size_query);
        #endif // HEAP_INFO_GET

        #if UART1_INFO_GET
            liot_uart_errcode_e ret_uart_get_dcbconfig;
            my_uart_config uart1_dcbconfig;

            ret_uart_get_dcbconfig = liot_uart_get_dcbconfig(LIOT_UART_PORT_1, &uart1_dcbconfig);
            if (ret_uart_get_dcbconfig != 0) 
            {
                liot_trace("liot_dev_memory_size_query Error Code:0x%X", ret_uart_get_dcbconfig);
            }    
            
            liot_trace("------------UART1 INFO------------");
            liot_trace("ret_uart_get_dcbconfig:0x%X", ret_uart_get_dcbconfig);
            liot_trace("Uart1_baudrate:%d", uart1_dcbconfig.baudrate);
            #if 0
            liot_trace("Uart1_data_bit:%d", uart1_dcbconfig.data_bit);
            liot_trace("Uart1_flow_ctrl:%d", uart1_dcbconfig.flow_ctrl);
            liot_trace("Uart1_isPrintfPort:%d", uart1_dcbconfig.isPrintfPort);
            liot_trace("Uart1_parity_bit:%d", uart1_dcbconfig.parity_bit);
            liot_trace("Uart1_stop_bit:%d", uart1_dcbconfig.stop_bit);
            #endif
        #endif // UART1_INFO_GET

    }
}