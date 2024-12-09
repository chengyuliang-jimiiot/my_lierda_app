/**
 * @File Name: my_app.c
 * @brief  
 * @Author : chengyuliang email:chengyuliang@jimiiot.com
 * @Version : 1.0
 * @Update : 2024-12-06
 **
 */

#include <stdio.h>
#include <string.h>
#include "stdlib.h"

#include "my_mem_ctrl.h"
#include "my_log.h"

#include "liot_os.h"
#include "liot_dev.h"



void liot_custom_demo_thread (void *argument)
{
    char *my_buffer = my_create_buffer();

    while(1)
    {   
        if (my_buffer != NULL)
        {
            my_simple_str_log("my_create_buffer success!_Mylog");
        }
        my_simple_str_log("HelloWorld!_Mylog");
        //liot_dev_get_imei();
        liot_rtos_task_sleep_ms(5000);
    }
}