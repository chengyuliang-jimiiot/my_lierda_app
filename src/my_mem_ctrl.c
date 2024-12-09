/**
 * @File Name: my_mem_ctrl.c
 * @brief  
 * @Author : chengyuliang email:chengyuliang@jimiiot.com
 * @Version : 1.0
 * @Update : 2024-12-06
 **
 */

#include <stdio.h>
#include <string.h>
#include "stdlib.h"

#include "my_log.h"

#include "my_mem_ctrl.h"

void my_free_buffer(char *buffer)
{
    if (buffer != NULL)
    {
        free(buffer);
        buffer = NULL;
    }
}

char* my_create_buffer(void)
{
    char *buffer = (char *)malloc(MY_BUFFER_SIZE * sizeof(char));
    if (buffer == NULL) 
    {
        my_simple_str_log("heap allocation failure");
        return NULL;
    }
    memset(buffer, 0, MY_BUFFER_SIZE);
    return buffer;
}