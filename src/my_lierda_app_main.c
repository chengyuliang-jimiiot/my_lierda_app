/**
 * @File Name: my_lierda_app_main.c
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
#include "my_dev.h"

/* Lierda Header */
#include "lierda_app_main.h"

/* macro definition */
#define SMALL_BUFFER_SIZE 16
#define NORMAL_BUFFER_SIZE 32
#define LARGE_BUFFER_SIZE 64

void liot_custom_demo_thread (void *argument)
{
    liot_errcode_dev_e ret;

    char *imei_buffer = my_create_buffer(NORMAL_BUFFER_SIZE);
    char *firmware_version_buffer = my_create_buffer(NORMAL_BUFFER_SIZE);
    char *sn_buffer = my_create_buffer(NORMAL_BUFFER_SIZE);
    char *product_id_buffer = my_create_buffer(NORMAL_BUFFER_SIZE);
    char *firmware_subversion_buffer = my_create_buffer(NORMAL_BUFFER_SIZE);
    char *model_buffer = my_create_buffer(NORMAL_BUFFER_SIZE);

    ret = liot_dev_get_imei(imei_buffer, NORMAL_BUFFER_SIZE, 0);
    if (ret != 0) 
    {
        liot_trace("liot_dev_get_imei Error Code:%d", ret);
    }
    ret = liot_dev_get_firmware_version(firmware_version_buffer, NORMAL_BUFFER_SIZE);
    if (ret != 0) 
    {
        liot_trace("liot_dev_get_firmware_version Error Code:%d", ret);
    }
    ret = liot_dev_get_sn(sn_buffer, NORMAL_BUFFER_SIZE, 0);
    if (ret != 0) 
    {
        liot_trace("liot_dev_get_sn Error Code:%d", ret);
    }
    ret = liot_dev_get_product_id(product_id_buffer, NORMAL_BUFFER_SIZE);
    if (ret != 0) 
    {
        liot_trace("liot_dev_get_product_id Error Code:%d", ret);
    }
    ret = liot_dev_get_firmware_subversion(firmware_subversion_buffer, NORMAL_BUFFER_SIZE);
    if (ret != 0) 
    {
        liot_trace("liot_dev_get_firmware_subversion Error Code:%d", ret);
    }
    ret = liot_dev_get_model(model_buffer, NORMAL_BUFFER_SIZE);
    if (ret != 0) 
    {
        liot_trace("liot_dev_get_model Error Code:%d", ret);
    }

    while(1)
    {   
        liot_rtos_task_sleep_ms(5000);
        liot_trace("IMEI(SIM0):%s", imei_buffer);
        liot_trace("firmware_version:%s", firmware_version_buffer);
        liot_trace("SN(SIM0):%s", sn_buffer);
        liot_trace("product_id:%s", product_id_buffer);
        liot_trace("firmware_subversion:%s", firmware_subversion_buffer);
        liot_trace("model:%s", model_buffer);
    }
}