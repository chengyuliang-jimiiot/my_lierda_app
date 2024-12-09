/**
 * @File Name: my_main.c
 * @brief  
 * @Author : chengyuliang email:chengyuliang@jimiiot.com
 * @Version : 1.0
 * @Update : 2024-12-09
 **
 */

#include <stdio.h>
#include <string.h>
#include "stdlib.h"

/* My header */
#include "my_mem_ctrl.h"
#include "my_log.h"
#include "my_os_ctrl.h"


void liot_custom_demo_thread (void *argument)
{
    char *my_buffer = my_create_buffer();
    while(1)
    {   
        if (my_buffer != NULL)
        {
            my_simple_str_log("my_create_buffer success!_Mylog");
        }
        my_simple_str_log("V2_Mylog");
        liot_rtos_task_sleep_ms(5000);
        liot_trace("1");

    }
}