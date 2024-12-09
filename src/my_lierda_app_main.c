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
#define MINI_BUFFER_SIZE 8
#define SMALL_BUFFER_SIZE 16
#define NORMAL_BUFFER_SIZE 32
#define LARGE_BUFFER_SIZE 64

void liot_custom_demo_thread (void *argument)
{
    liot_errcode_dev_e ret_get_imei, ret_get_firmware_version, ret_get_sn, ret_get_product_id, ret_get_firmware_subversion, ret_get_model, ret_memory_size_query, ret_get_modem_fun;
    liot_memory_heap_state_s heap_state;

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
    ret_memory_size_query = liot_dev_memory_size_query(&heap_state);
    if (ret_memory_size_query != 0) 
    {
        liot_trace("liot_dev_memory_size_query Error Code:%d", ret_memory_size_query);
    }
    ret_get_modem_fun = liot_dev_get_modem_fun(&modem_fun, 0);
    if (ret_get_modem_fun != 0) 
    {
        liot_trace("liot_dev_memory_size_query Error Code:%d", ret_get_modem_fun);
    }

    while(1)
    {   
        liot_rtos_task_sleep_ms(5000);
        liot_trace("IMEI(SIM0):%s  |ret=%d", imei_buffer, ret_get_imei);
        liot_trace("firmware_version:%s  |ret=%d", firmware_version_buffer, ret_get_firmware_version);
        liot_trace("SN(SIM0):%s  |ret=%d", sn_buffer, ret_get_sn);
        liot_trace("product_id:%s  |ret=%d", product_id_buffer, ret_get_product_id);
        liot_trace("firmware_subversion:%s  |ret=%d", firmware_subversion_buffer, ret_get_firmware_subversion);
        liot_trace("model:%s  |ret=%d", model_buffer, ret_get_model);
        liot_trace("modem_fun(SIM0):%d  |ret=%d", modem_fun, ret_get_modem_fun);
        liot_trace("total_heapsize:%d and avail_heapsize:%d  |ret=%d", heap_state.total_size, heap_state.avail_size, ret_memory_size_query);
    }
}