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

#include "my_log.h"

#include "lierda_app_main.h"

#ifdef __LIERDA_OCPU_API_H__
/*
*   @进行简单的字符串LOG输出
*   @str:要输出的字符串内容
*/
void my_simple_str_log(const char *str)
{
    if (str != NULL) {
        liot_trace("%s\n", str);
    } else {
        liot_trace("String is NULL\n");
    }    
}

#endif //利尔达