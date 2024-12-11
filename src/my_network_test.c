/**
 * @File Name: my_network_test.c
 * @brief  
 * @Author : chengyuliang email:chengyuliang@jimiiot.com
 * @Version : 1.0
 * @Update : 2024-12-11
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

void my_network_test_thread (void *argument)
{
    liot_task_t cur_taskRef;
    liot_rtos_task_get_current_ref(&cur_taskRef);

    int cid                        = 1;
    uint8_t nSim = 0;

    if(my_network_init(nSim, cid, LIOT_DATA_TYPE_IP, "APNTEST", "", "", LIOT_DATA_AUTH_TYPE_NONE) == false)
    {
        liot_trace("http liot_network_init failed!!!!");
        return;
    }

    while(1)
    {
        liot_rtos_task_sleep_ms(5000);
        liot_trace("my_network_test_stack_space = %d", liot_rtos_task_get_stack_space(cur_taskRef));
    }
}