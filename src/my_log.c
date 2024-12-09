/**
 * @File Name: my_log.c
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
#include "my_log.h"

/* Lierda Header */
#include "lierda_app_main.h"

/*
*   @���м򵥵��ַ���LOG���
*   @str:Ҫ������ַ�������
*/
void my_simple_str_log(const char *str)
{
    #ifdef __LIERDA_OCPU_API_H__
    if (str != NULL) {
        liot_trace("%s", str);
    } else {
        liot_trace("String is NULL");
    }
    #elif
    #endif // Lierda    
}


