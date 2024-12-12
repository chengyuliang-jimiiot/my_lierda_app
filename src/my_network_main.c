/**
 * @File Name: my_network_main.c
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
#include "my_mem_ctrl.h"
#include "my_log.h"
#include "my_os_ctrl.h"
#include "my_dev.h"
#include "my_uart.h"
#include "my_network.h"
#include "my_http.h"
#include "my_file.h"

#define HTTP_TASK_STACK_SIZE 1024*4

liot_task_t g_http_taskRef = NULL;
liot_task_t g_network_main_taskRef = NULL;
int cid = 1;
uint8_t nSim = 0;


void my_network_test_thread (void *argument)
{
    uint32_t natmode;
    int ret_iplocfile = -1;
    char *buffer = my_create_buffer(256);

    liot_rtos_task_get_current_ref(&g_network_main_taskRef);

    if(my_network_init(nSim, cid, LIOT_DATA_TYPE_IP, "APNTEST", "", "", LIOT_DATA_AUTH_TYPE_NONE) == false)
    {
        liot_trace("http liot_network_init failed!!!!");
        return;
    }

    liot_datacall_get_nat(nSim, &natmode);

    
    liot_rtos_task_create(&g_http_taskRef, HTTP_TASK_STACK_SIZE, APP_PRIORITY_NORMAL, "My_HTTP_Task", my_http_task_thread, NULL);

    while(1)
    {
        liot_rtos_task_sleep_ms(5000);
        liot_trace("my_network_test_stack_space = %d", liot_rtos_task_get_stack_space(g_network_main_taskRef));
        liot_trace("natmode:%d", natmode);
        ret_iplocfile = liot_file_exist(IP_LOC_INFO_FILE);
        liot_trace("check iploc file exist. ret === (%d)", ret_iplocfile);
        if (ret_iplocfile == 0)
        {
            my_open_file(IP_LOC_INFO_FILE, buffer);
        }
        else 
        {
            liot_trace("file does not exist!");
        }

    }
}